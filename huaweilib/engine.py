#!/usr/bin/env python
# -*- coding: latin-1 -*-
from huawei_API import *

class OntsManager:
	def __init__(self, ipaddr, username, password, port) :
		self.ipaddr = ipaddr
		self.username = username
		self.password = password
		self.port = port
		self.logger = Logger(fname="engine.log")

		self._oltConnCalls = 0
		self._oltConn = None

	# Comprueba si la conexión con la OLT está ocupada para poder enviar comandos nuevos.
	# Internamente no se usa porque el único propósito sería para lanzar un error. Así 
	# que veo más práctico que se use sólo desde fuera, antes de invocar funciones de aquí.
	def is_busy(self) :
		return self._oltConn and self._oltConn.is_busy()

	def connectOLT(self) :
		self._oltConnCalls += 1
		if not self._oltConn :
			self._oltConn = HuaweiOLTConn(self.ipaddr, self.username, self.password, self.port)
		self._oltConn.connect()

	def disconnectOLT(self, sure=False) :
		self._oltConnCalls -= 1
		if self._oltConnCalls <= 0 or sure :
			self._oltConn.disconnect()
			self._oltConnCalls = 0

	def disconnectAll(self) :
		disconnectOLT(self, True)

	def get_connections(self) :
		return self._oltConnCalls

	def get_last_error(self) :
		return self._oltConn.get_last_error()

	# TODO: Obtener esta informacion de la OLT
	def get_all_frames(self) :
		return ["0"]

	# TODO: Obtener esta informacion de la OLT
	def get_all_slots(self, frame) :
		return ["0"]
	
	# TODO: Obtener esta informacion de la OLT
	def get_all_gpons(self, frame, slot) :
		return ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"]

	# Obtiene una lista, ordenada por el puerto, con las ONTs administradas
	# Intento descubir la informacion de las ONT
	# Las lineas interesantes son las que empiezan con algo como "  0/ 0/0", donde:
	# el primer 0 es el Frame (creo que se refiere a la OLT).
	# el segundo 0 es el Slot (numero de la tarjeta)
	# El tercer 0 es el numero del GPON.
	# no se si la cantidad de espacios en blanco cambia si se tienen mas de 9 tarjetas.
	# Por cada ONT llegan dos lineas de informacion. La primera linea es la que incluye
	# el numero de serie y la segunda linea solo la descripcion.
	def get_managed(self, frames=None, slots=None) :
		self.connectOLT()

		if frames == None :
			frames = self.get_all_frames()
		elif not isinstance(frames, list) :
			frames = [frames]

		managed = [ ]
		for frame in frames :
			if slots == None :
				_slots = self.get_all_slots(frame)
			elif not isinstance(slots, list) :
				_slots = [slots]
			else :
				_slots = [slots]

			for slot in _slots :
				ctrl = {}
				infoText = self._oltConn.get_managed( frame, slot )
				if not infoText :
					break
				lines = infoText.splitlines()
				ont = None
				for line in lines :
					match = re.search(" +(\d+)/ *(\d+)/(\d+) +(\d+)", line)
					if match :
						ctrlkey = "%s/%s-%s" % (match.group(2),match.group(3),match.group(4))
						if not ctrl.has_key(ctrlkey) :#Nueva ONT encontrada.
							words = line[15:].split()
							HuaweiAPIError.check( len(words) == 6, "Formato de linea erroneo sacando el estado de las ONT administradas.\nLinea:%s\nQuiza es una version diferente a la testeada" % (line) )
							ont = ONTInfo()
							ont.frame = match.group(1)
							ont.slot = match.group(2)
							ont.gpon = match.group(3)
							ont.ID = match.group(4)
							ont.SN = words[0]
							ont.ctrlFlag = words[1]
							ont.runState = words[2]
							ont.cnfgState = words[3]
							ont.matchState = words[4]
							ont.protectSide = words[5]
							ctrl[ctrlkey] = ont
							managed += [ont]
						else :
							# Es la segunda vez que se encuentra el ID, asi que solo lleva la descripcion
							ctrl[ctrlkey].description = line[19:]

		self.disconnectOLT()
		return managed

	# Obtiene una lista, ordenada por el puerto, con las ONTs nuevas no administradas
	def get_unmanaged(self) :
		self.connectOLT()
		unmanaged = [ ]

		out = self._oltConn.get_unmanaged()
		if out :
			# este sistema de analisis solo funciona si la primera linea es 'Number' y ultima 'Ont autofind time'
			for line in out.splitlines():
				# Cada linea de datos tiene algo como <que es> : <que valor tiene>
				# Separo ambas partes de la linea, compruebo que haya realmente dos partes,
				# quito los espacios en blanco del principio y final de cada una de las dos partes
				pair = line.split(":")
				if len(pair) < 2 :
					continue
				key = pair[0].strip()
				# No se me ha dado el caso, pero por si hay mas veces ':' en la linea, vuelvo a juntarlo todo
				# por ejemplo, si hubiera una mac asi: xx:xx:xx:xx:xx:xx, en 'pair' habria muchos elementos
				# de esta manera, se vuelven a colocar como antes.
				value = ":".join(pair[1:]).strip()
				if key == "Number":
					ontInfo = ONTInfo()
					ontInfo.ID = "Nueva"
				elif key == "F/S/P" :
					ontInfo.frame, ontInfo.slot, ontInfo.gpon = value.replace(" ", "").split("/")
				elif key == "Ont SN" :
					ontInfo.SN = value
				elif key == "VendorID" :
					ontInfo.vendorID = value
				elif key == "Ont EquipmentID" :
					ontInfo.model = value
				elif key == "Ont Version" :
					# Un BUG en la OLT de Televés, que agrega caracteres no válidos y el django/python se hacen la pixa un lío...
					# Aunque no se hicieran un lío, tampoco funcionaría porque no coincide el ont_version de aquí con el que
					# retorna cuando está administrada.
					ontInfo.ont_version = re.sub("[^\w\.\-_]", "", value)
				elif key == "Ont SoftwareVersion" :
					ontInfo.firmware = value
				elif key == "Ont autofind time" :
					unmanaged += [ontInfo]

		self.disconnectOLT()
		return unmanaged

	def get_ont_mac_info(self, frame, slot, gpon, ontid) :
		self.connectOLT()
		wanInfoList = None
		text = self._oltConn.get_ont_mac_info(frame, slot, gpon, ontid)
		if text :
			wanInfoList = WanInfoList(frame, slot, gpon, ontid)

			for line in text.splitlines() :
				if (len(line) >= 53) and (line[16:20]=="gpon") :
					wanInfo = WanInfo()
					wanInfo.mac = line[21:35].replace("-", "")
					wanInfo.vlanID = int(line[len(line)-5:].replace(" ", ""))
					wanInfoList.addWanInfo(wanInfo)

		self.disconnectOLT()
		return wanInfoList

	# Obtiene la informacion de las interficies WAN de la ONT pasada.
	# Intentara obtenerlas a traves de un comando que, creo, es OMCI
	# Si falla, se buscara la informacion de las MAC/VLAN a traves
	# de la caracteristica que tiene la OLT de descubrirlas.
	def get_ont_wan_info(self, frame, slot, gpon, ontid) :
		self.connectOLT()
		wanInfoList = 0
		text = self._oltConn.get_ont_wan_info(frame, slot, gpon, ontid)
		if text :
			wanInfo = 0
			wanInfoList = WanInfoList(frame, slot, gpon, ontid)
			for line in text.splitlines() :
				pair = line.replace(" ", "").split(":")
				if len(pair) == 2 :
					if pair[0] == "Index":
						if wanInfo :
							wanInfoList.addWanInfo(wanInfo)
						wanInfo = WanInfo()
					elif pair[0] == "Name":
						wanInfo.name = pair[1]
					elif pair[0] == "Servicetype" :
						wanInfo.serviceType = pair[1]
					elif pair[0] == "Connectiontype" :
						wanInfo.connType = pair[1]
					elif pair[0] == "IPv4accesstype" :
						wanInfo.IPv4AccessType = pair[1]
					elif pair[0] == "IPv4address" :
						wanInfo.IPv4Address = pair[1]
					elif pair[0] == "Subnetmask" :
						wanInfo.mask = pair[1]
					elif pair[0] == "Defaultgateway" :
						wanInfo.gateway = pair[1]
					elif pair[0] == "ManageVLAN" :
						wanInfo.vlanID = int(pair[1])
					elif pair[0] == "Managepriority" :
						wanInfo.vlanPriority = int(pair[1])
					elif pair[0] == "MACaddress" :
						wanInfo.mac = pair[1].replace("-", "")
					elif pair[0] == "IPv6address" :
						wanInfo.IPv6Address = pair[1]
			if wanInfo :
				wanInfoList.addWanInfo(wanInfo)

		# Si no ha sacado la informacion completa de las WAN, intento obtener sus MAC-VLAN
		if not wanInfoList:
			wanInfoList = self.get_ont_mac_info(frame, slot, gpon, ontid)

		self.disconnectOLT()
		return wanInfoList

	def get_gpon_srv_profiles(self) :
		self.connectOLT()
		profiles = None

		out = self._oltConn.get_gpon_srv_profiles()
		if out :
			lines = out.splitlines()
			for line in lines :
				if len(line) > 59 and line[2].isdigit() :
					srvProf = SrvProfile()
					srvProf.id = line[2:14].strip()
					srvProf.name = line[14:58].strip()
					srvProf.bind_times = line[58:].strip()
					if not profiles :
						profiles = {}
					profiles[srvProf.id] = srvProf

		self.disconnectOLT()
		return profiles

	def get_gpon_srv_profile(self, srv_profile_id):
		self.connectOLT()

		srvProf = None
		out = self._oltConn.get_gpon_srv_profile(srv_profile_id)
		if out :
			srvProf = SrvProfile()
			srvProf.raw = out
			for line in srvProf.raw.splitlines() :
				if line.startswith("  Profile-ID") :
					srvProf.id = line[16:]
				elif line.startswith("  Profile-name") :
					srvProf.name = line[16:]
				elif line.startswith("  Access-type") :
					srvProf.access_type = line[16:]
				elif line.startswith("  POTS") :
					if not srvProf.pots :
						srvProf.pots = line[16:]
				elif line.startswith("  ETH") :
					if not srvProf.eth :
						srvProf.eth = line[16:]
				elif line.startswith("  VDSL") :
					if not srvProf.vdsl :
						srvProf.vdsl = line[16:]
				elif line.startswith("  TDM") :
					if not srvProf.tdm :
						srvProf.tdm = line[16:]
				elif line.startswith("  MOCA") :
					if not srvProf.moca :
						srvProf.moca = line[16:]
				elif line.startswith("  CATV") :
					if not srvProf.catv :
						srvProf.catv = line[16:]
				elif line.startswith("  Binding times") :
					srvProf.bind_times = line[18:]
					break

		self.disconnectOLT()
		return srvProf

	# Obtiene toda la informacion de las traffic ip. Como en la informacion basica
	# que devuelve la OLT no incluye el nombre, sacare tambien la informacion de
	# cada una de ellas.
	def get_traffic_tables(self):
		self.connectOLT()

		out = self.send_cmd(CMD_GET_TRAFFIC_TABLE_IP_LIST)
		tables = None
		if out :
			lines = out.splitlines()
			tables = {}
			for line in lines :
				if len(line) > 15 and line[5].isdigit() :
					table = self.get_traffic_table(line[2:6].strip())
					tables[table.index] = table

		self.disconnectOLT()
		return tables

	def get_traffic_table(self, table_index) :
		self.connectOLT()

		table = None
		out = self._oltConn.get_traffic_table(table_index)
		if out :
			table = TrafficTableIP()
			table.raw = out
			for line in table.raw.splitlines() :
				if line.startswith("  TD Index") :
					table.index = line[25:]
				elif line.startswith("  TD Name") :
					table.name = line[25:]
				elif line.startswith("  Priority  ") :
					table.priority = line[25:]
				elif line.startswith("  CIR") :
					table.cir = line[25:].replace(" kbps", "")
				elif line.startswith("  PIR") :
					table.pir = line[25:].replace(" kbps", "")

		self.disconnectOLT()
		return table

	def add_traffic_table(self, tt) :
		if tt.index :
			return self.mod_traffic_table(tt)
		self.connectOLT()
		out = self._oltConn.add_traffic_table( tt ) 
		self.disconnectOLT()
		if out :
			match = re.search("  TD Index             : (\d+)", out)
			tt.index = match.group(1)
			return tt
		return None

	def mod_traffic_table(self, tt) :
		self.connectOLT()

		HuaweiAPIError.check( tt.index, "Para modificar una tabla, debes pasar el indice de ella" )
		out = self._oltConn.mod_traffic_table( tt )

		self.disconnectOLT()
		if out :
			return tt
		return None

	def get_dba_profiles(self) :
		self.connectOLT()
		dbas = None
		out = self._oltConn.get_dba_profiles()
		if out :
			dbas = { }
			lines = out.splitlines()
			for line in lines :
				words = line.split()
				if len(words) == 7 and words[0].isdigit() and words[1].isdigit() :
					dba = DBAProfile()
					dba.profile_id, dba.qos_type, dba.bw_compensation, dba.bw_fix, dba.dw_assure, dba.bw_max, dba.bind_times = words
					out = self._oltConn.get_dba_profile( dba.profile_id )
					if not out :
						self.logger.error("No se ha podido sacar la informacion del dba-profile ID=%s" % (dba.profile_id))
						continue
					dba.raw = out
					match = re.search("Profile-name[ :]+([\w|\-]+)", out)
					if not match :
						self.logger.error("No se ha podido obtener el nombre del dba-profile ID=%s" % (dba.profile_id))
						continue
					dba.name = match.group(0)
					dbas[dba.profile_id] = dba

		self.disconnectOLT()
		return dbas

	def get_dba_profile(self, dba_profile_id) :
		profiles = self.get_dba_profiles()
		for dba in profiles :
			if dba.profile_id == dba_profile_id :
				return dba
		return None

	def add_dba_profile(self, dba) :
		self.connectOLT()
		out = self._oltConn.add_dba_profile(dba)
		self.disconnectOLT()
		if not out :
			return None
		match = re.search("Profile ID[ :]+(\d+)", out)
		if not match :
			return None
		dba.profile_id = match.group(1)
		return dbaprofile

	def del_dba_profile(self, profile_id) :
		self.connectOLT()
		tmp = self._oltConn.del_dba_profile(profile_id)
		self.disconnectOLT()
		return tmp

	def get_ont_service_ports(self, frame, slot, gpon, ontid):
		self.connectOLT()
		out = self._oltConn.get_ont_service_ports(frame, slot, gpon, ontid)
		sspp = None
		if out :
			sspp = {}
			for line in out.splitlines() :
				match = re.search(" +(\d+) +(\d+) +\w+ +\w+ +\d+/\d+ */\d+ +\d+ +(\d+) +\w+ +(\d+) +([-\d]+) +([-\d]+)", line)
				if match :
					sp = ServicePort()
					sp.index   = match.group(1)
					sp.oltVLAN = match.group(2)
					sp.gemport = match.group(3)
					sp.ontVLAN = match.group(4)
					sp.dwTrafficIndex = match.group(5)
					sp.upTrafficIndex = match.group(6)
					sspp[sp.index] = sp
		self.disconnectOLT()
		return sspp

	def get_ont_service_port(self, frame, slot, gpon, ontid, ont_vlan, olt_vlan) :
		self.connectOLT()
		rtn = None
		ports = self.get_ont_service_ports(frame, slot, gpon, ontid)
		if ports :
			# Para que no retorne un "None" que indicaria un error en el comando cuando solo es que no existe este service port
			# TODO: Mirar que hacer, ya que no esta claro que esta sea la mejor solucion.
			rtn = ServicePort()
			for port in ports.values() :
				if (port.ontVLAN == ont_vlan) and (port.oltVLAN == olt_vlan) :
					rtn = port
					break
		self.disconnectOLT()
		return rtn

	def del_service_port(self, service_port_index) :
		self.connectOLT()
		tmp = self._oltConn.del_service_port(service_port_index)
		self.disconnectOLT()
		return tmp

	def add_ont_service_port(self, frame, slot, gpon, ontid, service_port) :
		self.connectOLT()
		tmp = self._oltConn.add_ont_service_port(frame, slot, gpon, ontid, service_port)
		self.disconnectOLT()
		return tmp

	def manage_ont(self, frame, slot, gpon, ontsn, desc, srv_profile_id, line_profile_id, tr069_profile_id, gest_service_port, other_service_ports=tuple() ) :
		self.connectOLT()
		kwargs = { }
		kwargs["frame"] = frame
		kwargs["slot"] = slot
		kwargs["gpon"] = gpon
		kwargs["SN"] = ontsn
		kwargs["desc"] = desc
		kwargs["srv_profile_id"] = srv_profile_id
		kwargs["line_profile_id"] = line_profile_id
		kwargs["gest_vlan"] = gest_service_port.ontVLAN
		kwargs["tr069_profile_id"] = tr069_profile_id

		ontid = self._oltConn.manage_ont(**kwargs)
		if ontid :
			self.add_ont_service_port(frame, slot, gpon, ontid, gest_service_port)

			for service_port in other_service_ports :
				self.add_ont_service_port(frame, slot, gpon, ontid, service_port)

		self.disconnectOLT()
		return ontid

	def unmanage_ont(self, frame, slot, gpon, ontid):
		self.connectOLT()
		# Saco los service-ports de la ONT.
		sspp = self.get_ont_service_ports(frame, slot, gpon, ontid)
		if sspp :
			for sp in sspp.values() :
				self.del_service_port(sp.index)

		out = self._oltConn.unmanage_ont(frame, slot, gpon, ontid)
		self.disconnectOLT()
		return out

	'''
	MA5683T(config)#display ont version 0 0 15 0
	  -------------------------------------------------------------------------- 
	  F/S/P                    : 0/0/15 
	  ONT-ID                   : 0 
	  Vendor-ID                : PTIN 
	  ONT Version              : ONTRGW1269A002 
	  Product-ID               : 0 
	  Equipment-ID             : GR2412GA 
	  Main Software Version    : 3RGW030600u148 
	  Standby Software Version :  
	  OntProductDescription    :  
	  Support XML Version      :  
	  -------------------------------------------------------------------------- 

	MA5683T(config)#display ont version 0 0 3.0 
	                                        ^
	  % Parameter error, the error locates at '^'

	MA5683T(config)#display ont version 0 0 3 0
	  -------------------------------------------------------------------------- 
	  F/S/P                    : 0/0/3 
	  ONT-ID                   : 0 
	  Vendor-ID                : HWTC 
	  ONT Version              : 494.B 
	  Product-ID               : 49 
	  Equipment-ID             : 245H 
	  Main Software Version    : V3R013C00S106 
	  Standby Software Version : V3R013C00S106 
	  OntProductDescription    : EchoLife HG8245H GPON Terminal (CLASS B+/PRODUC 
	                             T ID:2102310QXF6RG4002232) 
	  Support XML Version      : 1.3.0.0 
	  -------------------------------------------------------------------------- 
	'''
	def get_ont_version(self, ont=None, frame=None, slot=None, gpon=None, ontid=None) :
		self.connectOLT()
		if ont != None:
			frame = ont.frame
			slot = ont.slot
			gpon = ont.gpon
			ontid = ont.ID
		else :
			ont = ONTInfo()
			ont.frame = frame
			ont.slot = slot
			ont.gpon = gpon
			ont.ID = ontid

		out = self._oltConn.get_ont_version(frame, slot, gpon, ontid)
		if not out :
			ont = None
		else :
			for line in out.splitlines() :
				pair = line.split(":")
				if len(pair) >= 2 :
					if pair[0].startswith("  Vendor-ID") :
						ont.vendorID = (str(":").join(pair[1:])).strip()
					elif pair[0].startswith("  Product-ID") :
						ont.product_id = (str(":").join(pair[1:])).strip()
					elif pair[0].startswith("  ONT Version") :
						ont.ont_version = (str(":").join(pair[1:])).strip()
					elif pair[0].startswith("  Equipment-ID") :
						ont.model = (str(":").join(pair[1:])).strip()
					elif pair[0].startswith("  Main Software Version") :
						ont.firmware = (str(":").join(pair[1:])).strip()

		self.disconnectOLT()
		return ont

	def set_tr069_profile_id(self, frame, slot, gpon, ontid, tr069_profile_id) :
		self.connectOLT()
		out = self._oltConn.set_tr069_profile_id(gpon, ontid, tr069_profile_id, frame, slot)
		self.disconnectOLT()
		return out

	def reset_ont(self, frame, slot, gpon, ontid) :
		self.connectOLT()
		out = self._oltConn.reset_ont(gpon, ontid, frame, slot)
		self.disconnectOLT()
		return out

	def reboot_ont(self, frame, slot, gpon, ontid) :
		self.connectOLT()
		out = self._oltConn.reboot_ont(gpon, ontid, frame, slot)
		self.disconnectOLT()
		return out

def check_args(cmd, argv, extras=None) :
	if extras == None :
		extras = list()
	if len(argv) != len(extras) + 6 :# Los 6 argumentos son: el comando, action, frame, slot, gpon y ontid
		raise Exception("Ussage: engine.py %s <frame> <slot> <gpon> <ontid> <%s>" % (cmd, "".join(extras)))
	if extras :
		print "Setting '%s' as %s on ONT %s/%s/%s-%s" % ("".join(extras), argv[6], argv[2], argv[3], argv[4], argv[5])
	else :
		print "Aplying '%s' on ONT %s/%s/%s-%s" % (cmd, argv[2], argv[3], argv[4], argv[5])

	return argv[2:]

if __name__ == '__main__':

	import sys
	ontsManager = OntsManager("10.50.0.3", "root", "admin", 23)

#	print ontsManager.get_speed_profiles()
#	quit()
	ontID = None

	if "print" in sys.argv :
		out = ontsManager.get_managed()
		if out :
			for ont in out :
				print "%s/%s/%s-%s, %s (%s): %s" % (ont.frame, ont.slot, ont.gpon, ont.ID, ont.SN, ont.description, ont.runState)
		else :
			print "No managed ONTs"
		exit()
	if "tr069" in sys.argv :
		frame, slot, gpon, ontid, tr069_id = check_args("tr069", sys.argv, ["tr069_profile_id"])
		print ontsManager.set_tr069_profile_id(frame, slot, gpon, ontid, tr069_id)
		exit()
	if "factory" in sys.argv :
		frame, slot, gpon, ontid = check_args("reset", sys.argv )
		print ontsManager.reset_ont(frame, slot, gpon, ontid)
		exit()
	if "reboot" in sys.argv :
		frame, slot, gpon, ontid = check_args("reboot", sys.argv )
		print ontsManager.reboot_ont(frame, slot, gpon, ontid)
		exit()
	if "backup" in sys.argv :
		print ontsManager.current_config()
		exit()
	if "add" in sys.argv :
		kwargs = {}
		kwargs["frame"] = "0"
		kwargs["slot"] = "0"
		kwargs["gpon"] = "0"
		kwargs["ontsn"] = "48575443A2145564"
		kwargs["desc"] = "68"
		kwargs["srv_profile_id"] = "0" # 5 para huawei, 10 para Televes
		kwargs["line_profile_id"] = "5" # 5 para 1TCONT, 10 para 1TCONT por GEMPORT
		kwargs["tr069_profile_id"] = "2"

		# Para la gestion de la ONT.
		gest_sp = ServicePort()
		gest_sp.gemport = "0"
		gest_sp.ontVLAN = "3333"
		gest_sp.oltVLAN = "3333"
		gest_sp.dwTrafficIndex = "15"
		gest_sp.upTrafficIndex = "15"

		# Si hay internet.
		inet_sp = ServicePort()
		inet_sp.gemport = "1"
		inet_sp.ontVLAN = "110"
		inet_sp.oltVLAN = "110"
		inet_sp.dwTrafficIndex = "15"
		inet_sp.upTrafficIndex = "15"

		kwargs["gest_service_port"] = gest_sp
		kwargs["other_service_ports"] = (inet_sp, )

		ontID = ontsManager.manage_ont(**kwargs)
		if not ontID :
			quit()
		print "ONT administrada con el ID: %s" % (ontID)
	if "del" in sys.argv :
		frame, slot, gpon, ontid = check_args("del", sys.argv)
		print ontsManager.unmanage_ont(frame, slot, gpon, ontid)
	else :
		print "uso: print|tr069|factory|reboot|backup|add|del [args]"


#	pp = ontsManager.get_speed_profiles()
#	if pp :
#		print "Mostrando %d perfil(es) encontrados:" % (len(pp))
#		for p in pp.values() :
#			print p.__str__()
#	else:
#		print "Ningun perfil de velocidad"
#		spA = SpeedProfile()
#		spA.name = "30/30"
#		spA.downloadAssured = "2048"
#		spA.downloadTotal = "32576"
#		spA.downloadPriority = "5"
#		spA.uploadAssured = "2048"
#		spA.uploadTotal = "32576"
#		spA.uploadPriority = "5"
#		ontsManager.set_speed_profile(spA)

'''
Comandos Para la ONT de televés.

ont-srvprofile gpon profile-id 20 profile-name "Tlv76502"
  ont-port pots 2 eth 4 catv 1
  port vlan iphost translation 110 user-vlan 110
  port vlan iphost translation 120 user-vlan 120
  port vlan iphost translation 3333 user-vlan 3333
  commit

ont-lineprofile gpon profile-id 6 profile-name "TlvTContsGem0"
  tr069-management enable
  tcont 0 dba-profile-id 20
  tcont 1 dba-profile-id 21
  tcont 2 dba-profile-id 22
  gem add 0 eth tcont 0
  gem add 1 eth tcont 1
  gem add 2 eth tcont 2
  gem mapping 0 0 vlan 3333
  gem mapping 1 0 vlan 110
  gem mapping 2 0 vlan 120
#  gem mapping 1 1 vlan 110
#  gem mapping 2 2 vlan 120
  commit
  quit

ont-lineprofile gpon profile-id 21 profile-name "TlvTContGem0"
  tr069-management enable
  tcont 1 dba-profile-id 12
  gem add 0 eth tcont 1
  gem add 1 eth tcont 1
  gem add 2 eth tcont 1
  gem mapping 0 0 vlan 3333
  gem mapping 1 0 vlan 110
  gem mapping 2 0 vlan 120
#  gem mapping 1 1 vlan 110
#  gem mapping 2 2 vlan 120
  commit
  quit

[Para dar de alta la ONT]
interface gpon 0/0
#ont add 15 sn-auth 5054494E3076EDAF omci ont-lineprofile-id 6 ont-srvprofile-id 20 desc "TelevesTest"
ont add 15 sn-auth 5054494E3076EDAF omci ont-lineprofile-id 21 ont-srvprofile-id 20 desc "TelevesTest"
ont ipconfig 15 0 ip-index 0 dhcp vlan 3333 priority 5

quit
service-port vlan 3333 gpon 0/0/15 ont 0 gemport 0 multi-service user-vlan 3333 tag-transform translate 

'''
