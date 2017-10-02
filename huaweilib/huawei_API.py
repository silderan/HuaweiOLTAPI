# -*- coding: latin-1 -*-
#!/usr/bin/env python
import re
from telnetlib import Telnet
from logger import Logger
from huawei_cnfg import *
from huawei_cmds import *
from data_classes import *

class HuaweiOLTConn(object) :
	def __init__(self, ipaddr, username, password, port=22):
		self.ipaddr = ipaddr
		self.username = username
		self.password = password
		self.port = port
		self.oltConn = 0
		self.lastError = ""
		self.logger = Logger()
		self.logger.add_tag("SendCmd")
		self.logger.set_log_file("ConnLog")
		self.connecting = False
		self.sending_cmd = "" # El comando que está enviándose actualmente.

	def __del__(self):
		self.disconnect()

	# Comprueba si la API está ocupada.
	def is_busy(self) :
		return (self.connecting == True) or (self.sending_cmd != "")

	def isConnected(self):
		if not self.oltConn :
			return False
		#TODO: Comprobar que la conexion se mantiene activa.
		return True

	def _connect_lazy(self) :
		HuaweiAPIError.check( self.username, "Falta el nombre de usuario" )
		HuaweiAPIError.check( self.ipaddr, "Falta la direccion IP de la OLT" )
		HuaweiAPIError.check( self.password, "Falta la contrasenya de usuario" )
		HuaweiAPIError.check( self.port, "Falta el puerto de conexion a la OLT" )
		self.logger.info( "Conectando a %s@%s:%s" % (self.username, self.ipaddr, self.port) )
		self.oltConn = Telnet(self.ipaddr, self.port, LOGIN_TIMEOUT)
		HuaweiAPIError.check( self.oltConn, "No se ha podido conectar con la OLT" )

		out = self.oltConn.read_until( PROMT_LOGIN, LOGIN_TIMEOUT )
		HuaweiAPIError.check( out.find( BAD_USERNAME ) < 0, "%s (El usuario %s no existe)" % (BAD_USERNAME, self.username) )
		HuaweiAPIError.check( out.find( BAD_LOGIN ) < 0, "%s (El usuario %s no es válido)" % (BAD_LOGIN, self.username) )
		HuaweiAPIError.check( out.find( ASK_FOR_USERNAME ) >= 0, ASSERT_BAD_USER_ASK_TEXT )
		self.oltConn.write( buffer(self.username+"\n") )

		out = self.oltConn.read_until( PROMT_LOGIN, LOGIN_TIMEOUT )
		HuaweiAPIError.check( out.find( BAD_USERNAME ) < 0, BAD_USERNAME )
		HuaweiAPIError.check( out.find( BAD_LOGIN ) < 0, BAD_LOGIN )
		HuaweiAPIError.check( out.find( ASK_FOR_PASSWORD ) >= 0, ASSERT_BAD_PASS_ASK_TEXT )
		self.oltConn.write( buffer(self.password+"\n") )

		#Espero el primer prompt
		out = self.oltConn.read_until(PROMPT, CMD_TIMEOUT)
		HuaweiAPIError.check( out.find( BAD_LOGIN ) < 0, BAD_LOGIN )
		HuaweiAPIError.check( out.find( TOO_MANY_USER_CONNECTIONS ) < 0, TOO_MANY_USER_CONNECTIONS )
		self.logger.info("Conectado a %s@%s:%s" % (self.username, self.ipaddr, self.port) )

	def connect(self):
		if (not self.connecting) and (not self.isConnected()) :
			self.connecting = True
			try :
				self._connect_lazy()
			except :
				self.disconnect()
				raise
			self.connecting = False
			self._send_cmd(CMD_NO_PAGGING)
			self._send_cmd(CMD_ENABLE_CONFIG)
			self._send_cmd(CMD_ENTER_CONFIG)
		return None

	def disconnect(self):
		if self.isConnected() :
			try:
				self.oltConn.close()
			except:
				raise
			finally:
				self.logger.info("Desconectado de %s@%s:%s" % (self.username, self.ipaddr, self.port) )
				self.oltConn = 0
				self.connecting = False
				self.sending_cmd = ""

	def _send_cmd_lazy(self, prompt) :
		self.logger.logit( "SendCmd", self.sending_cmd )
		self.oltConn.write( buffer("%s\n" % self.sending_cmd) )

		out = self.oltConn.read_until(prompt, CMD_TIMEOUT)
		if out.find(prompt) < 0 :
			raise HuaweiAPIError("No se ha recibido la respuesta completa desde la OLT")
		HuaweiAPIError.check( out, "La OLT no ha retornado ningun texto. Quizá se ha perdido la conexion" )
		for error in COMMAND_ERRORS.keys() :
			if out.find(error) >= 0 :
				self.lastError = "Error en el comando %s:%s\nLa OLT responde: '%s'" % (self.sending_cmd, COMMAND_ERRORS[error], out)
				self.logger.error(self.lastError)
				return None

		return out

	def _send_cmd(self, cmd, prompt=PROMPT):
		HuaweiAPIError.check( self.connecting == False, "Aun no se ha completado la conexion con la OLT y se intenta enviar el comando '%s'!" % (cmd) )
		HuaweiAPIError.check( self.sending_cmd == "", "Aun esta enviandose el comando '%s' anterior y se intenta enviar el comando '%s'!" % (self.sending_cmd, cmd) )
		try :
			self.sending_cmd = cmd
			out = self._send_cmd_lazy(prompt)
			self.sending_cmd = ""
			return out
		except :
			self.disconnect()
			raise

	def get_last_error(self) :
		return self.lastError

	def _parse_cmd(self, cmd, params):
		for param in params.keys() :
			cmd = cmd.replace("{%s}" % (param), str(params[param]))

		return cmd

	def send_cmd(self, _cmd, cmdParser=None, prompt=PROMPT):
		self.connect()
		if cmdParser :
			_cmd = self._parse_cmd(_cmd, cmdParser)
		return self._send_cmd(_cmd)

	def get_managed(self, frame, slot) :
		return self.send_cmd( CMD_BOARD_INFO, {"frame":frame, "slot": slot} )

	def get_unmanaged(self) :
		return self.send_cmd( CMD_GET_UNMANAGED )

	def get_ont_wan_info(self, frame, slot, gpon, ontid):
		return self.send_cmd( CMD_GET_ONT_WAN_INFO, {"frame":frame, "slot":slot, "gpon":gpon, "ontid":ontid} )

	def get_ont_mac_info(self, frame, slot, gpon, ontid):
		return self.send_cmd( CMD_GET_ONT_MAC_INFO, {"frame":frame, "slot":slot, "gpon":gpon, "ontid":ontid} )

	def get_gpon_srv_profiles(self):
		return self.send_cmd(CMD_GET_GPON_SRV_PROFILES)

	def get_gpon_srv_profile(self, srv_profile_id):
		return self.send_cmd( CMD_GET_GPON_SRV_PROFILE, {"srv_profile_id": srv_profile_id} )

	def get_traffic_table(self, index) :
		return self.send_cmd( CMD_GET_TRAFFIC_TABLE_IP_INFO, {"index": index} )

	def get_traffic_tables(self) :
		return self.send_cmd( CMD_GET_TRAFFIC_TABLE_IP_LIST )

	def add_traffic_table(self, tt) :
		return self.send_cmd( CMD_ADD_TRAFFIC_TABLE_IP, {"cir": tt.cir, "pir": tt.pir, "priority": tt.priority, "name": tt.name} ) 

	def mod_traffic_table(self, tt) :
		HuaweiAPIError.check( tt.index, "Para modificar una tabla, debes pasar el indice de ella" )
		return self.send_cmd( CMD_MOD_TRAFFIC_TABLE_IP, {"index": tt.index, "cir": tt.cir, "pir": tt.pir, "priority": tt.priority, "name": tt.name} )

	def del_traffic_table(self, trafficTableIndex) :
		return self.send_cmd( CMD_DEL_TRAFFIC_TABLE_IP, {"index": trafficTableIndex} ) 

	def get_dba_profiles(self) :
		return self.send_cmd( CMD_GET_DBA_PROFILES )

	def add_dba_profile(self, dbaprofile) :
		# Como la OLT no da una informacion detalla del error, lo hago aqui asi
		if len(dbaprofile.bw_fix) and len(dbaprofile.bw_assure) and len(dbaprofile.bw_max) :
			if int(dbaprofile.bw_fix) > int(dbaprofile.bw_assure):
				raise DBAException("La velocidad maxima debe ser menor a la velocidad asegurada")
			if int(dbaprofile.bw_assure) > int(dbaprofile.bw_max):
				raise DBAException("La velocidad asegurada debe ser menor a la velocidad maxima")
			tipo = "type5"
			speed = "fix %s assure %s max %s" % (dbaprofile.bw_fix, dbaprofile.bw_assure, dbaprofile.bw_max)
		elif len(dbaprofile.bw_fix) == 0 and len(dbaprofile.bw_assure) == 0 and len(dbaprofile.bw_max) :
			tipo = "type4"
			speed = "max %s" % (dbaprofile.bw_max)
		elif len(dbaprofile.bw_fix) == 0 and len(dbaprofile.bw_assure) and len(dbaprofile.bw_max) :
			if  int(dbaprofile.bw_assure) > int(dbaprofile.bw_max):
				raise DBAException("La velocidad asegurada debe ser menor a la velocidad maxima")
			tipo = "type3"
			speed = "assure %s max %s" % (dbaprofile.bw_assure, dbaprofile.bw_max)
		elif len(dbaprofile.bw_fix) == 0 and len(dbaprofile.bw_assure) and len(dbaprofile.bw_max) == 0 :
			tipo = "type2"
			speed = "assure %s" % (dbaprofile.bw_assure)
		elif len(dbaprofile.bw_fix) and len(dbaprofile.bw_assure) == 0 and len(dbaprofile.bw_max) == 0 :
			if (dbaprofile.bw_compensation != "yes") and (dbaprofile.bw_compensation != "no"):
				raise DBAException("Cuando el tipo de DBA es 1, se debe indicar si se compensa el ancho de banda o no (yes/no)")
			tipo = "type1"
			speed = "fix %s bandwidth_compensate %s" % (dbaprofile.bw_fix, dbaprofile.bw_compensation)
		else :
			raise DBAException("La combinacion de velocidades para el DBA %s no es valida para ningun tipo" % (dbaprofile.name))
		return self.send_cmd( CMD_ADD_DBA_PROFILE, {"name": dbaprofile.name, "id": dbaprofile.profile_id, "type": tipo, "speeds":speed} )

	def del_dba_profile(self, dbaprofile_id) :
		return self.send_cmd( CMD_DEL_DBA_PROFILE, {"id": dbaprofile_id} )

	def get_ont_service_ports(self, frame, slot, gpon, ontid) :
		return self.send_cmd( CMD_GET_ONT_SERVICE_PORTS, {"frame":frame, "slot":slot, "gpon":gpon, "ontid":ontid} )

	def del_service_port(self, service_port_index) :
		return self.send_cmd( CMD_DEL_SERVICE_PORT, {"index":service_port_index} )

	def add_ont_service_port(self, frame, slot, gpon, ontid, service_port) :
		HuaweiAPIError.check( service_port.oltVLAN, "Falta la VLAN de la OLT" )
		HuaweiAPIError.check( service_port.ontVLAN, "Falta la VLAN de la ONT" )
		if service_port.dwTrafficIndex and service_port.upTrafficIndex :
			return self.send_cmd( CMD_ADD_SERVICE_PORT_LIMITED,\
													 {"frame":frame, "slot":slot, "gpon":gpon, "ontid": ontid,\
													 	"gem":service_port.gemport, "olt_vlan":service_port.oltVLAN, "ont_vlan":service_port.ontVLAN,\
													 	"dw_traffic_index":service_port.dwTrafficIndex, "up_traffic_index":service_port.upTrafficIndex} )
		else:
			return self.send_cmd( CMD_ADD_SERVICE_PORT_UNLIMITED,\
													 {"frame":frame, "slot":slot, "gpon":gpon, "ontid": ontid,\
													 	"gem":service_port.gemport, "olt_vlan":service_port.oltVLAN, "ont_vlan":service_port.ontVLAN} )

	def enter_GPON_interface(self, frame, slot) :
		return self.send_cmd( CMD_ENTER_GPON_CONTEXT, {"frame": frame, "slot": slot})

	def quit_GPON_interface(self) :
		return self.send_cmd( "quit" )

	# Una funcion auxiliar interna para reducir codigo muchas veces repetido.
	# Solo puede usarse por funciones envian comandos unicos al gpon indicado o no importa que
	# entre/salga cada vez.
	def _send_GPON_interface_cmd(self, cmd, parse_data, frame=None, slot=None) :
		if frame and slot :
			self.enter_GPON_interface(frame, slot)
		out = self.send_cmd( cmd, parse_data )
		if frame and slot :
			self.quit_GPON_interface()
		return out

	def reset_ont(self, gpon, ontid, frame=None, slot=None) :
		return self._send_GPON_interface_cmd( CMD_RESET_ONT, {"gpon": gpon, "ontid": ontid}, frame, slot )

	def reboot_ont(self, gpon, ontid, frame=None, slot=None) :
		return self._send_GPON_interface_cmd( CMD_REBOOT_ONT, {"gpon": gpon, "ontid": ontid, "gracetime": "30"}, frame, slot )

	def set_tr069_profile_id(self, gpon, ontid, tr069_profile_id, frame=None, slot=None) :
		return self._send_GPON_interface_cmd( CMD_SET_TR069_SERVER, {"gpon": gpon, "ontid": ontid, "tr069id": tr069_profile_id}, frame, slot )

	def create_ont_gest_interface(self, gpon, ontid, gest_vlan, tr069_profile_id, gest_priority=5, frame=None, slot=None) :
		return self._send_GPON_interface_cmd( CMD_SET_ONT_IPCONFIG, {"gpon": gpon, "ontid": ontid, "vlan": gest_vlan, "priority": gest_priority}, frame, slot )

	def delete_ont_gest_interface(self, gpon, ontid, frame=None, slot=None) :
		return self._send_GPON_interface_cmd( CMD_UNSET_ONT_IPCONFIG, {"gpon": gpon, "ontid": ontid}, frame, slot )

	def manage_ont(self, frame, slot, gpon, SN, desc, srv_profile_id, line_profile_id, gest_priority=5, gest_vlan=None, tr069_profile_id=None) :
		HuaweiAPIError.check_port("manage_ont", frame, slot, gpon)
		HuaweiAPIError.check_positive_num("manage_ont: srv_profile_id", srv_profile_id)
		HuaweiAPIError.check_positive_num("manage_ont: line_profile_id", line_profile_id)
		HuaweiAPIError.check_positive_num("manage_ont: gest_priority", gest_priority)
		if gest_vlan != None :
			HuaweiAPIError.check_positive_num("manage_ont: gest_vlan", gest_vlan)
		if gest_vlan != None :
			HuaweiAPIError.check_positive_num("manage_ont: tr069_profile_id", tr069_profile_id)

		self.enter_GPON_interface(frame, slot)

		out = self.send_cmd( CMD_MANAGE_ONT, {"gpon": gpon, "desc": desc, "SN": SN, "srvprofile": srv_profile_id, "lineprofile": line_profile_id} )
		if not out :
			return None
		match = re.search("ONTID :([0-9]+)", out)
		HuaweiAPIError.check( match, "No se ha podido saber el ID de la ONT anyadida: %s" % (out) )
		ontid = match.group(1)
		HuaweiAPIError.check( ontid.isdigit(), "El ID obtenido de la ONT anyadida no es valido: %s" % (ontid) )

		if tr069_profile_id :
			self.set_tr069_profile_id(gpon, ontid, tr069_profile_id)

		if gest_vlan :
			self.create_ont_gest_interface(gpon, ontid, gest_vlan, gest_priority)

		self.quit_GPON_interface()
		return ontid

	def unmanage_ont(self, frame, slot, gpon, ontid) :
		HuaweiAPIError.check_full_id("unmanage_ont", frame, slot, gpon, ontid)
		return self._send_GPON_interface_cmd( CMD_UNMANAGE_ONT, {"gpon": gpon, "ontid": ontid}, frame, slot )

	def get_ont_version(self, frame, slot, gpon, ontid) :
		HuaweiAPIError.check_full_id("get_ont_version", frame, slot, gpon, ontid)
		return self.send_cmd( CMD_GET_ONT_VERSION, {"frame":frame, "slot":slot, "gpon":gpon, "ontid":ontid} )
