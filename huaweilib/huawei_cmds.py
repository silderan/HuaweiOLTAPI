# -*- coding: latin-1 -*-
## Estos son los textos del "prompt" Si no se especifican correctamente
## el programa se quedara bloqueado o funcionara mal al no saber cuando
## dejar de esperar datos del servidor.

# El "prompt" que se muestra al pedir el usuario o contrasenya
PROMT_LOGIN = ":"
# El texto que se muestra cuando se pude el usuario
ASK_FOR_USERNAME = "User name"
# El texto que se muestra al pedir la contrasenya.
ASK_FOR_PASSWORD = "User password"
# En el login, cuando no se puede conectar de nuevo con el mismo usuario
TOO_MANY_USER_CONNECTIONS = "Reenter times have reached the upper limit."
# El "prompt" que se muestra despues de cada comando ejecutado
PROMPT = "\nMA5683T"

# Textos que se envian con ciertos errores a traves de 'assert'
ASSERT_BAD_USER_ASK_TEXT = "No se ha recibido el texto que pide el usuario (%s)" % ASK_FOR_USERNAME
ASSERT_BAD_PASS_ASK_TEXT = "No se ha recibido el texto que pide la contrasenya (%s)" % ASK_FOR_PASSWORD

## Mensajes varios que muestra la OLT en caso de error.
# Cuando se pone un usuario o contrasenya no autorizados
BAD_LOGIN = "Username or password invalid."
# Cuando se pasa un usuario vacio (supongo que por caracteres no validos)
BAD_USERNAME = "Username or Domain invalid!"

## Errores al enviar comandos. Es un texto de error que devuelve la OLT.
COMMAND_ERRORS = {"Unknown command" : "Comando desconocido o no se esta en el contexto correcto",\
									"Failure" : "Fallo al ejecutar el comando",\
									"Parameter error": "Parametro del comando erroneo mal escrito o con caracteres no validos",\
									"Incomplete command": "El comando esta incompleto. Faltan parametros o son erroneos",\
									"Too many parameters": "Demasiados argumentos en el comando"}

## Comandos a enviar al telnet
# Para que no pagine el texto.
CMD_NO_PAGGING = "scroll 512"
# Para poder configurar la maquina
CMD_ENABLE_CONFIG = "enable"
# Para entrar/salir en el modo de configuracion
CMD_ENTER_CONFIG = "config"
CMD_EXIT_CONFIG = "quit"
# Para obtener la informacion global de las ONT
CMD_BOARD_INFO = "display board {frame}/{slot}"
# Para obtener la informacion de las ONT nuevas
CMD_GET_UNMANAGED = "display ont autofind all"
# Para obtener la informacion de IPs, VLANs y MAC de la wan de la OLT
CMD_GET_ONT_WAN_INFO = "display ont wan-info {frame}/{slot} {gpon} {ontid}"
# Para obtener las MAC-VLAN que la OLT descubre de las ONT.
CMD_GET_ONT_MAC_INFO = "display mac-address port {frame}/{slot}/{gpon} ont {ontid}"
# Para obtener la tabla de los "srv-profiles"
CMD_GET_GPON_SRV_PROFILES = "display ont-srvprofile gpon all"
# Para obtener la informacion de un service profile concreto.
CMD_GET_GPON_SRV_PROFILE = "display ont-srvprofile gpon profile-id {srv_profile_id}"
# Para obtener informacion sobre la version de hardware, software, etc.
CMD_GET_ONT_VERSION = "display ont version {frame} {slot} {gpon} {ontid}"

# Para manipular los traffic table
CMD_GET_TRAFFIC_TABLE_IP_INFO = "display traffic table ip index {index}"
CMD_ADD_TRAFFIC_TABLE_IP = "traffic table ip name \"{name}\" cir {cir} pir {pir} priority {priority} priority-policy local-Setting"
CMD_MOD_TRAFFIC_TABLE_IP = "traffic table ip modify index {index} rename \"{name}\" cir {cir} pir {pir} priority {priority} priority-policy local-Setting\n"
CMD_DEL_TRAFFIC_TABLE_IP = "undo traffic table ip index {index}"

# Para obtener los dba-profiles
CMD_GET_DBA_PROFILES = "display dba-profile all"
CMD_GET_DBA_PROFILE = "display dba-profile profile-id {id}"
CMD_ADD_DBA_PROFILE = "dba-profile add profile-name \"{name}\" {type} {speeds}"
CMD_DEL_DBA_PROFILE = "dba-profile delete profile-id {id}"

# Para activar/desactivar la gestion del ACS tr069 en las ONT:
CMD_SET_TR069_SERVER = "ont tr069-server-config {gpon} {ontid} profile-id {tr069id}"
CMD_UNSET_TR069_SERVER = "undo ont tr069-server-config {gpon} {ontid}"
# Para activar/desactivar la interficie de gestion en las ONT
CMD_SET_ONT_IPCONFIG = "ont ipconfig {gpon} {ontid} dhcp vlan {vlan} priority {priority}"
CMD_UNSET_ONT_IPCONFIG = "undo ont ipconfig {gpon} {ontid}\n"
# Para administrar/dejar de administrar una ONT en la OLT
CMD_MANAGE_ONT = "ont add {gpon} sn-auth \"{SN}\" omci ont-lineprofile-id {lineprofile} ont-srvprofile-id {srvprofile} desc \"{desc}\" "
CMD_UNMANAGE_ONT = "ont delete {gpon} {ontid}"
# Para entrar/salir en el contexto de configuracion de los GPON
CMD_ENTER_GPON_CONTEXT = "interface gpon {frame}/{slot}"
CMD_QUIT_GPON_CONTEXT = "quit"
# Para crear/borrar/mostrar los service-port
CMD_GET_ONT_SERVICE_PORTS = "display service-port port {frame}/{slot}/{gpon} ont {ontid}\n"
CMD_ADD_SERVICE_PORT_LIMITED = "service-port vlan {olt_vlan} gpon {frame}/{slot}/{gpon} ont {ontid} gemport {gem} multi-service user-vlan {ont_vlan} tag-transform translate inbound traffic-table index {up_traffic_index} outbound traffic-table index {dw_traffic_index}"
CMD_ADD_SERVICE_PORT_UNLIMITED = "service-port vlan {olt_vlan} gpon {frame}/{slot}/{gpon} ont {ontid} gemport {gem} multi-service user-vlan {ont_vlan} tag-transform translate\n"
CMD_DEL_SERVICE_PORT = "undo service-port {index}"
# Para resetear a valores de fábrica la ONT por OMCI
CMD_RESET_ONT = "ont factory-setting-restore {gpon} {ontid}\ny"
# Para reiniciar la ONT
CMD_REBOOT_ONT = "ont reset graceful {gpon} {ontid} {gracetime}\ny"

# Para crear/borrar/mostrar los lineprofile de gpon/epon
# TODO: Completarlo.
CMD_GET_LINEPROFILES = "display ont-lineprofile {gepon} all"
CMD_GET_LINEPROFILE = "display ont-lineprofile {gepon} profile-id {profile} profile-name \"{name}\""
CMD_ADD_LINEPROFILE = "ont-lineprofile {gepon} profile-name \"{name}\""
CMD_DEL_LINEPROFILE = "undo ont-lineprofile {gepon} profile-id {profile}"
CMD_MOD_LINEPROFILE = "ont-lineprofile {gepon} profile-id \"{name}\""
CMD_LINEPROFILE_TR069 = " tr069-management {enable_disable}"
CMD_LINEPROFILE_TCONT_DBA = "tcont {tcont_index} dba-profile-id {dba_profile_id}"
CMD_LINEPROFILE_ADD_GEM = "gem add {gemport} eth tcont {tcont_index}"
CMD_LINEPROFILE_GEM_MAP = "gem mapping 0 0 vlan {vlan}"
