# -*- coding: latin-1 -*-
from huawei_cnfg import *
# Aquí se incluyen todas las clases con los datos y algunas funciones
# para su manipulación.
# También se incluyen algunas funciones globales para simplificar y unificar el
# acceso y uso de datos.

def createDefaultWiFi(ontInfo, pre_ssid=DEFAULT_PRE_SSID, ssid_len=6, ssid_pre="", ssid_pos="", pass_len=8, pass_pre="", pass_pos=""):
	wifi = WiFiData()
	wifi.enabled = True
	wifi.ssid, wifi.wpa = toWiFiData(ontInfo.SN, ssid_len, ssid_pre, ssid_pos, pass_len, pass_pre, pass_pos )

	if ontInfo.wifi.caps.bandw_2Ghz:
		wifi.bandw2Ghz = ontInfo.wifi.caps.bandw_2Ghz[len(ontInfo.wifi.caps.bandw_2Ghz)-1]

	if ontInfo.wifi.caps.bandw_5Ghz:
		wifi.bandw2Ghz = ontInfo.wifi.caps.bandw_5Ghz[len(ontInfo.wifi.caps.bandw_5Ghz)-1]

	if "WPA" in ontInfo.wifi.caps.WPAEncryptionModes:
		wifi.WPAMode = "WPA"
	else:
		wifi.WPAMode = ontInfo.wifi.caps.WPAEncryptionModes[0]

	if "PSK" in ontInfo.wifi.caps.WPAEncryptionModes:
		wifi.WPAAuth = "PSK"
	else:
		wifi.WPAAuth = ontInfo.wifi.caps.WPAAuthModes[0]

	return wifi

def createAllDefaultWiFi(ontInfo):
	# Por cada red wifi que pueda crear, pongo la configuracion que tiene realmente la ONT o, si no la tiene,
	# la configuracion por defecto.
	if not ontInfo.wifi.networks :
		ontInfo.wifi.networks = list()
	while len(ontInfo.wifi.networks) < ontInfo.wifi.caps.maxWiFis :
		if len(ontInfo.wifi.networks) :
			extra = str(len(ontInfo.wifi.networks))
		else :
			extra = ""
		ontInfo.wifi.networks.append( createDefaultWiFi(ontInfo, ssid_pre="Novatel_", ssid_pos=extra, pass_pre=extra) )

# Crea un SSID y pass a partir del numero de serie de la ONT
def toWiFiData(SN, ssid_len=6, ssid_pre="", ssid_pos="", pass_len=8, pass_pre="", pass_pos=""):
	ssid =""
	i = len(SN)-1
	while i >= 0 and len(ssid) < ssid_len:
		ssid += SN[i]
		i-=1
	ssid = "%s%s%s" % (ssid_pre, ssid, ssid_pos)

	password = ""
	i = 0
	while len(password) < pass_len :
		password += SN[i%len(SN)]
		i+=3
	password = "%s%s%s" % (pass_pre, password, pass_pos)
	return (ssid, password)


def prettyMAC(mac,splitChar=":",groupLen=2):
		# TODO: algoritmizarlo!!!!
		return "%s:%s:%s:%s:%s:%s" % (mac[0:2],mac[2:4],mac[4:6],mac[6:8],mac[8:10],mac[10:12])



class DBAException(Exception):
	pass

class HuaweiAPIError(Exception) :
	@staticmethod
	def check(_test, message) :
		if not _test :
			raise HuaweiAPIError(message)

	@staticmethod
	def check_positive_num(msg, n) :
		if n == None :
			raise HuaweiAPIError("%s: su valor es 'None'" % msg)
		try:
			n = int(n)
		except:
			raise HuaweiAPIError("%s: no es un numero %s" % (msg, str(n)) )
		if n < 0 :
			raise HuaweiAPIError("%s: es un numero negativo (%d)" % (msg, n))

	@staticmethod
	def check_port(msg, frame, slot, gpon) :
		HuaweiAPIError.check_positive_num("%s, frame" % msg, frame)
		HuaweiAPIError.check_positive_num("%s, slot" % msg, slot)
		HuaweiAPIError.check_positive_num("%s, gpon" % msg, gpon)

	@staticmethod
	def check_full_id(msg, frame, slot, gpon, ontid) :
		HuaweiAPIError.check_positive_num("%s, frame" % msg, frame)
		HuaweiAPIError.check_positive_num("%s, slot" % msg, slot)
		HuaweiAPIError.check_positive_num("%s, gpon" % msg, gpon)
		HuaweiAPIError.check_positive_num("%s, ontid" % msg, ontid)

class WanInfo:
		def __init__(self):
				self.mac = ""
				self.vlanID = 0
				self.vlanPriority = 0
				self.IPv4Address = ""
				self.IPv4AccessType = ""
				self.IPv6Address = ""
				self.mask = ""
				self.gateway = ""
				self.connType = ""
				self.serviceType = ""
				self.name = ""
		def __str__(self):
				return "Nombre     : %s\n"\
							 "Servicio   : %s\n"\
							 "MAC        : %s\n"\
							 "VLAN       : %d (Prioridad: %d)\n"\
							 "Datos IPv4 : %s: %s, %s, %s\n"\
							 "Datos IPv6 : %s\n"\
						% ( self.name,\
								self.serviceType,\
								prettyMAC(self.mac),\
								self.vlanID, self.vlanPriority,\
								self.IPv4AccessType, self.IPv4Address, self.mask, self.gateway,\
								self.IPv6Address )

class WanInfoList(object):
		def __init__(self, frame, slot, gpon, ontid):
				self.wanList = {}
				self.frame = frame
				self.slot = slot
				self.gpon = gpon
				self.ontid = ontid

		def addWanInfo(self, wanInfo):
				assert isinstance(wanInfo, WanInfo), "wanInfo solo puede ser del tipo WanInfo"
				ivlan = int(wanInfo.vlanID)
				if ivlan > 0 :
						self.wanList[ivlan] = wanInfo
						return self.wanList[ivlan]

		def allWanInfos(self):
				return self.wanList.values()

		def allVLANS(self):
				return self.wanList.keys()

		def wanInfo(self, vlan):
				return self.wanList[int(vlan)]

		def __unicode__(self) :
			return self.__str__()
		def __str__(self):
			out = "Cantidad de interfaces WAN = %d\n" % len(self.wanList)
			for wan in self.allWanInfos() :
				out += str(wan)+"\n"
			return out;

class WiFiCap(object):
	def __init__(self):
		self.channels_2Ghz = list(["auto"])
		i = 1
		while i < 14:
			self.channels_2Ghz += list([str(i)])
			i+=1
		self.bandw_2Ghz = list(["20", "40"])

		self.channels_5Ghz = list(["auto"])
		i = 1
		while i < 10:
			self.channels_5Ghz += list(str(i))
			i+=1
		self.bandw_5Ghz = ("20", "40")

		self.maxClients = 16
		self.maxWiFis = 1		 # Cantidad de redes wifi que puede tener.
		self.WPAEncryptionModes = list(["WPA"])
		self.WPAAuthModes = list(["PSK"])

class WiFiData(object):
	def __init__(self):
		self.enabled = False
		self.ssid = ""
		self.wpa = ""
		self.chan2Ghz = ""
		self.bandw2Ghz = ""
		self.hidden = False
		self.WPAMode = ""
		self.WPAAuth = ""

class WiFi(object):
	def __init__(self):
		self.acs = True # Si esta configurada a traves del ACS.
		self.caps = WiFiCap()	 # Capacidades de WiFi de la ONT
		self.networks = None		# Lista de redes.

class SrvProfile(object):
	def __init__(self):
		self.raw =""
		self.id = ""
		self.name = ""
		self.bind_times = ""
		self.access_type = ""
		self.pots = ""
		self.eth = ""
		self.vdsl = ""
		self.tdm = ""
		self.moca = ""
		self.catv = ""

	def __str__(self):
		return "ID     : %s\n"\
					 "Name   : %s\n"\
					 "Binded : %s times" %\
					 (self.id,\
					 self.name,\
					 self.bind_times)
					 

class TrafficTableIP(object):
	def __init__(self):
		self.raw = ""
		self.index = ""
		self.name = ""
		self.cir = ""
		self.pir = ""
		self.priority = ""
	def __str__(self):
		return "Indice     : %s\n"\
					 "Nombre     : %s\n"\
					 "CIR        : %s KBps\n"\
					 "PIR        : %s KBps\n"\
					 "Prioridad  : %s\n"\
				% (self.index,\
					 self.name,\
					 self.cir,\
					 self.pir,\
					 self.priority)
	def __unicode__(self) :
		return self.__str__()

class DBAProfile(object):
	def __init__(self) :
		self.raw = ""
		self.name = ""
		self.profile_id = ""
		self.qos_type = ""
		self.bw_compensation = ""
		self.bw_fix = ""
		self.bw_assure = ""
		self.bw_max = ""
		self.bind_times = ""
	def __str__(self) :
		return "ID                     : %s\n"\
					 "Nombre                 : %s\n"\
					 "DBA-type               : %s\n"\
					 "Bandwidth compensation : %s\n"\
					 "Fixed Bandwidth        : %s\n"\
					 "Assured Bandwidth      : %s\n"\
					 "Maximun Bandwidth      : %s\n"\
					 "Usado                  : %s veces\n"\
				% (self.name,\
					 self.profile_id,\
					 self.qos_type,\
					 self.bw_compensation,\
					 self.bw_fix,\
					 self.bw_assure,\
					 self.bw_max,\
					 self.bind_times)

class ServicePort(object) :
	def __init__(self) :
		self.index = None
		self.oltVLAN = None
		self.ontVLAN = None
		self.dwTrafficIndex = None
		self.dwTrafficName = None
		self.upTrafficIndex = None
		self.upTrafficName = None
		self.gemport = None
	def __str__(self) :
		if self.upTrafficName :
			up = "ID=%s Name=%s" % (self.upTrafficIndex, self.upTrafficName)
		else :
			up = "ID=%s Name= --" % (self.upTrafficIndex)
		if self.dwTrafficName :
			dw = "ID=%s Name=%s" % (self.dwTrafficIndex, self.dwTrafficName)
		else :
			dw = "ID=%s Name= --" % (self.dwTrafficIndex)
		return "service-port Index           : %s\n"\
					 "OLT VLAN                     : %s\n"\
					 "ONT VLAN                     : %s\n"\
					 "inbound traffic-table        : %s (descarga)\n"\
					 "outbound traffic-table       : %s (carga)\n"\
					 "gemport id                   : %s\n"\
			 % (self.index,\
					self.oltVLAN,\
					self.ontVLAN,\
					dw,\
					up,\
					self.gemport)

class ONTInfo():
	def __init__(self):
		self.frame = ""
		self.slot = ""
		self.gpon = ""
		self.ID = ""
		self.SN = ""
		self.ctrlFlag = ""
		self.runState = ""
		self.cnfgState = ""
		self.matchState = ""
		self.protectSide = ""
		self.description = ""
		self.wanInfoList = None
		self.wifi = WiFi() # Informacion de las redes wifi.
		self.speedProfile = None
		self.vendorID = ""
		# Estos datos solo estan disponibles si se hace una consulta especifica
		self.product_id = None
		self.ont_version = None
		self.model = None
		self.firmware = None

	def __repr__(self) :
		return "%s/%s/%s-%s (%s, %s)" % (self.frame, self.slot, self.gpon, self.ID, self.SN, self.vendorID)
		return self.__str__()

	def __str__(self):
		return	"Port (F/S/P-ID)           : %s/%s/%s-%s\n"\
						"Serial Number, Fabricante : %s, %s\n"\
						"Descripcion               : %s\n"\
						"Running                   : %s\n"\
				 % (self.frame, self.slot, self.gpon, self.ID,\
						self.SN, self.vendorID,\
						self.description,\
						self.runState)

	def isOnline(self):
		return self.runState == "online"

