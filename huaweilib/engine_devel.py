#!/usr/bin/env python
from telnetlib import Telnet
import time
import re

#TODO: Pasarlo/usar libreria
# Una clase basica y simple para registrar cambios (la que tiene python es demasiado compleja)
# TODO: Anyadir la posibilidad de que se guarde en un fichero.
# TODO: Crear un "procesador" del texto del log para que retorne los datos en una estructura
#       y asi poderse mostrar "formateados" de la manera que el disenyador quiera.
class Logger(object):
    def __init__(self, tagsToLog=["Info", "Warning", "Error", "Critical", "Debug"], fname=""):
        assert isinstance(tagsToLog, list) or isinstance(tagsToLog, tuple), "Solo se pueden pasar listas"
        self.tagsToLog = list(tagsToLog)

    def setLogFile(self, fname):
        self.fname = fname;

    def addTag(self, tag):
        assert isinstance(tag, str), "Solo se pueden pasar cadenas"
        if tag not in self.tagsToLog :
            self.tagsToLog += [tag]

    def delTag(self, tag):
        assert isinstance(tag, str), "Solo se pueden pasar cadenas"
        if tag in self.tagsToLog :
            self.tagsToLog.remove(tag)

    def delTags(self, tags):
        assert isinstance(self.tagsToLog, list) or isinstance(self.tagsToLog, tuple), "Solo se pueden pasar listas"
        for tag in tags :
            self.delTag(tag)

    def addTags(self, tagsToLog):
        assert isinstance(self.tagsToLog, list) or isinstance(self.tagsToLog, tuple), "Solo se pueden pasar listas"
        for tag in tags :
            self.addTag(tag)

    def logit(self, level="Info", msg=""):
        if len(msg) and (not level or (level in self.tagsToLog)) :
            out = "%s [%s] %s" % (time.ctime(), level, msg)
            if self.fname:
                # TODO: !!
                pass
            print out

    def info(self, msg):
        self.logit("Info", msg)

    def warning(self, msg):
        self.logit("Warning", msg)

    def error(self, msg):
        self.logit("Error", msg)

    def critical(self, msg):
        self.logit("Critical", msg)

    def debug(self, msg):
        self.logit("Debug", msg)

class ConnLog(Logger):
    def __init__(self):
        Logger.__init__(self)
        self.addTag("SendCmd")
        self.setLogFile("ConnLog")

    def sendCmd(self, msg):
        self.logit("SendCmd", msg)

_conLogger = ConnLog()
# Si se quieren evitar mensajes de informacion.
# _conLogger.delTags( ("Warning", "Info") )

## Estos son los textos del "prompt" Si no se especifican correctamente
## el programa se quedara bloqueado o funcionara mal al no saber cuando
## dejar de esperar datos del servidor.

# El "prompt" que se muestra al pedir el usuario o contrasenya
PROMT_LOGIN = ":"
# El texto que se muestra cuando se pude el usuario
ASK_FOR_USERNAME = "User name"
# El texto que se muestra al pedir la contrasenya.
ASK_FOR_PASSWORD = "User password"
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

## Comandos a enviar al telnet

# Para que no pagine el texto.
CMD_NO_PAGGING = "scroll 512"
# Para poder configurar la maquina
CMD_ENABLE_CONFIG = "enable"
# Para obtener la informacion global de las ONT
CMD_BOARD_INFO = "display board {frame}/{slot}"
# Para obtener la informacion de las ONT nuevas
CMD_GET_UNMANAGED = "display ont autofind all"
# Para obtener la informacion de IPs, VLANs y MAC de la wan de la OLT
CMD_GET_ONT_WAN_INFO = "display ont wan-info {frame}/{slot} {gpon} {ontid}"
# Para obtener las MAC-VLAN que la OLT descubre de las ONT.
CMD_GET_ONT_MAC_INFO = "display mac-address port {frame}/{slot}/{gpon} ont {ontid}"

## Tiempo que tarda en considerar que la OLT no responde...
# tras enviarle el usuario o la contrasenya
LOGIN_TIMEOUT = 10
# Tiempo que tarda en considerar que la OLT no responde
CMD_TIMEOUT = 10

## Varios
# Primera ID para las ONT
ONTSID_FIRST = 0
# Ultima ID para las ONT
ONTSID_LAST = 127
# Rango de ids posibles por gpon
ONTIDS_RANGE = xrange(ONTSID_FIRST, ONTSID_LAST)
# UN ONTID no valido usado por algunas funciones.
ONTSID_INVALID = -1

class HuaweiOLTConn(object):
    def __init__(self, ipaddr, username, password, port=22):
        self.ipaddr = ipaddr
        self.username = username
        self.password = password
        self.port = port
        self.oltConn = 0
        self.priviledgedMode = False

    def __del__(self):
        self.disconnect()

    def isConnected(self):
        if( not self.oltConn ):
            return False
        #TODO: Comprobar que la conexion se mantiene activa.
        return True

    def connect(self):
        if( not self.isConnected() ):
            _conLogger.info("Conectando a %s@%s:%d" % (self.username, self.ipaddr, self.port) )
            self.oltConn = Telnet(self.ipaddr)
            assert self.oltConn, "No se ha podido conectar con la OLT"

            out = self.oltConn.read_until( PROMT_LOGIN, LOGIN_TIMEOUT )
            assert out.find( BAD_USERNAME ) < 0, BAD_USERNAME
            assert out.find( BAD_LOGIN ) < 0, BAD_LOGIN
            assert out.find( ASK_FOR_USERNAME ) >= 0, ASSERT_BAD_USER_ASK_TEXT
            self.oltConn.write(self.username+"\n")

            out = self.oltConn.read_until( PROMT_LOGIN, LOGIN_TIMEOUT )
            assert out.find( BAD_USERNAME ) < 0, BAD_USERNAME
            assert out.find( BAD_LOGIN ) < 0, BAD_LOGIN
            assert out.find( ASK_FOR_PASSWORD ) >= 0, ASSERT_BAD_PASS_ASK_TEXT
            self.oltConn.write(self.password+"\n")

            #Espero el primer prompt
            self.oltConn.read_until(PROMPT, CMD_TIMEOUT)
            _conLogger.info("Conectado a %s@%s:%d" % (self.username, self.ipaddr, self.port) )
            self._send_cmd(CMD_NO_PAGGING)
            self._send_cmd(CMD_ENABLE_CONFIG)

    def disconnect(self):
        if( self.isConnected() ):
            self.oltConn.close()
            _conLogger.info("Desconectado de %s@%s:%d" % (self.username, self.ipaddr, self.port) )
        self.priviledgedMode = False
        self.oltConn = 0

    def _send_cmd(self, cmd, prompt=PROMPT):
        _conLogger.sendCmd( cmd )
        self.oltConn.write( cmd+"\n" )
        out = self.oltConn.read_until(prompt, CMD_TIMEOUT)
        if (out.find("Parameter error") >= 0) or (out.find("Failure:") >= 0) :
            _conLogger.warning("Error en el comando %s:%s" % (cmd, out))
            return None
        return out

    def _parse_cmd(self, cmd, frame, slot, gpon, ontid):
        cmd = cmd.replace("{frame}", str(frame))
        cmd = cmd.replace("{slot}", str(slot))
        cmd = cmd.replace("{gpon}", str(gpon))
        cmd = cmd.replace("{ontid}", str(ontid))
        return cmd

    def send_cmd(self, cmd, prompt=PROMPT):
        self.connect()
        return self._send_cmd(cmd)

    def get_olt_board_info(self, frame="0", slot="0"):
        return self.send_cmd(self._parse_cmd(CMD_BOARD_INFO, frame, slot, 0, 0))

    def get_unmanaged_onts_info(self):
        return self.send_cmd(CMD_GET_UNMANAGED)

    # Obtiene la informacion de las interficies WAN de las ONT.
    # Si esta funcion falla, por modelos no compatibles con OMCI (creo),
    # se debe usar get_ont_mac_info() para obtener las MAC-VLAN
    # y consultar luego el router que otorga las direcciones IP
    def get_ont_wan_info(self, frame="0", slot="0", gpon="0", ontid="0"):
        return self.send_cmd(self._parse_cmd(CMD_GET_ONT_WAN_INFO, frame, slot, gpon, ontid))

    # Obtiene la informacion de las MAC-VLAN que la OLT
    # ha podido descubrir. Usada cuando falla get_ont_wan_info()
    def get_ont_mac_info(self, frame="0", slot="0", gpon="0", ontid="0"):
        return self.send_cmd(self._parse_cmd(CMD_GET_ONT_MAC_INFO, frame, slot, gpon, ontid))


def toOntFullID(olt_F, olt_S, olt_P, ont_ID):
    return "%d/%d/%d-%d" % (int(olt_F), int(olt_S), int(olt_P), int(ont_ID))

def fromFullID(fullID):
    return fullID.replace("-", "/").split("/")

def olt_F_fromFullID(fullID):
    return fromFullID(fullID)[0]

def olt_S_fromFullID(fullID):
    return fromFullID(fullID)[1]

def olt_P_fromFullID(fullID):
    return fromFullID(fullID)[2]

def ont_ID_fromFullID(fullID):
    return fromFullID(fullID)[3]

def prettyMAC(mac,splitChar=":",groupLen=2):
    # TODO: algoritmizarlo!!!!
    return "%s:%s:%s:%s:%s:%s" % (mac[0:2],mac[2:4],mac[4:6],mac[6:8],mac[8:10],mac[10:12])

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
    def __init__(self, ontFullID):
        self.wanList = {}
        self.ontFullID = ontFullID

    def ontFullID(self):
        return self.ontFullID

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

    def __str__(self):
        out = "Cantidad de interfaces WAN = %d\n" % len(self.wanList)
        for wan in self.allWanInfos() :
            out += str(wan)+"\n"
        return out;

class ONTInfo(dict):
    def __init__(self):
        self.vendorID = ""
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
        self._addresses = { }

    def toMap(self):
        return {"VendorID":self.vendorID,\
                "Frame": self.frame,\
                "Slot": self.slot,\
                "GPON": self.gpon,\
                "ONT-ID": self.ID,\
                "SN": self.SN,\
                "RunState":self.runState}

    def __str__(self):
        return  "FullID (F/S/P-ID)         : %s/%s/%s-%s\n"\
                "Serial Number, Fabricante : %s, %s\n"\
                "Descripcion               : %s\n"\
                "Running                   : %s\n"\
             % (self.frame, self.slot, self.gpon, self.ID,\
                self.SN, self.vendorID,\
                self.description,\
                self.runState)

    def isOnline(self):
        return self.runState == "online"

class ONTList:

    def __init__(self):
        self.ontsInfo = { }

    def exists(self, fullID):
        return self.ontsInfo.has_key(fullID)

    def __getitem__(self, fullID):
        if self.exists(fullID) :
            return self.ontsInfo[fullID]
        return None

    def __setitem__(self, fullID, ontInfo):
        self.ontsInfo[fullID] = ontInfo
        return self.ontsInfo[fullID]

    def __str__(self):
        return self.ontsInfo.__str__()

    def allONTs(self):
        return self.ontsInfo.values()

    def allFullIDs(self, ordered=True):
        keys = self.ontsInfo.keys()
        if( ordered ):
            # TODO: El orden deberia tener en cuenta los valores numericos y no el valor alfabetico
            # porque despues de 0/1/0 pondra 0/10/0 en vez de 0/2/0
            # Para ello, la funcion "sort" acepta como parametro una funcion, pero aun no se como hacerlo
            keys.sort()
        return keys

    def clear(self):
        self.ontsInfo = { }

    # Obtiene el siguiente ID disponible para el puerto (S/F/P-ID)
    # retorna ONTSID_INVALID si no queda ninguno disponible
    def getNextFreeID(self, frame, slot, gpon):
        for ID in ONTIDS_RANGE :
            if not self.exists(toOntFullID(frame, slot, gpon, ID)) :
                return str(ID)
        return ONTSID_INVALID

    def toMapList(self):
        mapList = list()
        for fullID in self.allFullIDs() :
            mapList += self.ontsInfo[fullID].toMap()
        return mapList;

class OntsManager:

    def __init__(self, ipaddr, username, password, port=22):
        self.ipaddr = ipaddr
        self.username = username
        self.password = password
        self.port = port

    def _get_HuaweiConn(self):
        return HuaweiOLTConn(self.ipaddr, self.username, self.password, self.port)

# 
# Intento descubir la informacion de las ONT
# Las lineas interesantes son las que empiezan con algo como "  0/ 0/0", donde:
# el primer 0 es el Frame (creo que se refiere a la OLT).
# el segundo 0 es el Slot (numero de la tarjeta)
# El tercer 0 es el numero del GPON.
# no se si la cantidad de espacios en blanco cambia si se tienen mas de 9 tarjetas.
# Por cada ONT llegan dos lineas de informacion. La primera linea es la que incluye
# el numero de serie y la segunda linea solo la descripcion.
#
    def _getManagedONTs(self, oltConn):
        # TODO: Analizar antes los frames/slots disponibles para que busque en todos ellos
        infoText = oltConn.get_olt_board_info(0, 0)
        if not infoText :
            return None
        lines = infoText.splitlines()
        managedONTList = ONTList()

        for line in lines:
            if re.search("[ 0-9][ 0-9][ 0-9]/", line) :
                ontPort = line[0:10].replace(" ", "").split("/")
                words = line[10:].split()
                ontID = words[0]
                assert len(ontPort) == 3, "Formato de linea erroneo sacando el FullID para saber las ONT administradas. Quiza es una version superior a la testeada"
                ontFullID = toOntFullID(ontPort[0], ontPort[1], ontPort[2], ontID)
                if not managedONTList.exists(ontFullID) :
                    # Es la primera vez que se ve esta FullID, asi que es la linea a) que incluye el SN.
                    assert len(words) > 6, "Formato de linea erroneo sacando el estado de las ONT administradas. Quiza es una version superior a la testeada"
                    ontInfo = ONTInfo()
                    ontInfo.frame = ontPort[0]
                    ontInfo.slot = ontPort[1]
                    ontInfo.gpon = ontPort[2]
                    ontInfo.ID = ontID
                    ontInfo.SN = words[1]
                    ontInfo.ctrlFlag = words[2]
                    ontInfo.runState = words[3]
                    ontInfo.cnfgState = words[4]
                    ontInfo.matchState = words[5]
                    ontInfo.protectSide = words[6]
                    # Anyado la informacion de la ont en la tabla hash
                    managedONTList[ontFullID] = ontInfo
                else:
                    # es la segunda vez que se ve esta FullID, asi que es la linea b) que solo lleva la descripcion
                    managedONTList[ontFullID].description = " ".join(words[1:])
        return managedONTList

    def getManagedONTs(self):
        oltConn = self._get_HuaweiConn()
        managedONTList = self._getManagedONTs(oltConn)
        oltConn.disconnect()
        return managedONTList

    def _getONTsMACInfo(self, oltConn, frame, slot, gpon, ontid) :
        text = oltConn.get_ont_mac_info(frame, slot, gpon, ontid)
        if not text :
            return None
        wanInfoList = WanInfoList(toOntFullID(frame, slot, gpon, ontid))
        for line in text.splitlines() :
            if (len(line) >= 53) and (line[16:20]=="gpon") :
                wanInfo = WanInfo()
                wanInfo.mac = line[21:35].replace("-", "")
                wanInfo.vlanID = int(line[len(line)-5:])
                wanInfoList.addWanInfo(wanInfo)

        return wanInfoList

# 
# La informacion de las WAN de la ONT
# Esta informacion se obtiene siempre directamente de la OLT, por
# eso no retornara de inmediato.
# Si retorna una lista vacia, es que no hay nada configurado en la ONT
# Si retorna None, es que la ONT esta offline
# Para que funcione, la primera palabra que encuentra en la informacion
# de la ONT ha de ser "index"
#
    def _getONTsWANInfo(self,oltConn, frame, slot, gpon, ontid):
        text = oltConn.get_ont_wan_info(frame, slot, gpon, ontid)
        if not text :
            return None

        wanInfo = 0
        wanInfoList = WanInfoList(toOntFullID(frame, slot, gpon, ontid))
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
        return wanInfoList

    def getONTWANInfo(self, frame, slot, gpon, ontid):
        oltConn = self._get_HuaweiConn()
        wanInfoList = self._getONTsWANInfo(oltConn, frame, slot, gpon, ontid)
        # Si no ha sacado la informacion completa de las WAN, intento obtener sus MAC-VLAN
        if not wanInfoList:
            wanInfoList = self._getONTsMACInfo(oltConn, frame, slot, gpon, ontid)
            assert wanInfoList, "No se ha podido sacar la informacion de la ONT %s. Quiza esta offline" % (toOntFullID(frame, slot, gpon, ontid))
        return wanInfoList

    def getUnmanagedONTs(self):
        huaweiConn = self._get_HuaweiConn()
        out = huaweiConn.get_unmanaged_onts_info()
        # este sistema de analisis solo funciona si la primera linea "Number:<num>" y la segunda es el F/S/P de cada ONT encontrada.
        index = 0;
        unmanagedONTs = ONTList()

        for line in out.splitlines():
            pair = line.split(":")
            if( len(pair) < 2 ):
                continue
            key = pair[0].strip()
            value = ":".join(pair[1:]).strip()
            if( key == "Number" ):
                ID = value
            elif( key == "F/S/P" ):
                index = value+"-"+ID
                unmanagedONTs[index] = ONTInfo()
                unmanagedONTs[index].ID = ID
                unmanagedONTs[index].frame, unmanagedONTs[index].slot, unmanagedONTs[index].gpon = value.replace(" ", "").split("/")
            elif( key == "Ont SN" ):
                unmanagedONTs[index].SN = value
            elif( key == "VendorID" ):
                unmanagedONTs[index].vendorID = value

        return unmanagedONTs

def test():
    onts_info = OntsManager("10.50.0.3", "root", "admin")

    # Para evitar flood
    _conLogger.delTags( ("Info", "Debug", "SendCmd") )

    # Saco informacion de las ONT administradas.
    managedONTs = onts_info.getManagedONTs()
    print "\nONTs administradas"
    for fullID in managedONTs.allFullIDs():
        ont = managedONTs[fullID]
        print "%s" % (ont)
        if ont.isOnline() :
            print onts_info.getONTWANInfo(ont.frame, ont.slot, ont.gpon, ont.ID)

    # Uso este mapping para guardar los puertos (F/S/P) donde hay ONTs descubiertas para
    # saber que ID hay libre en ellas.
    GPONS = { }
    # Saco la informacion de las ONTs nuevas
    unmanagedONTs = onts_info.getUnmanagedONTs()
    print "\nONTs nuevas"
    for fullID in unmanagedONTs.allFullIDs():
        ont = unmanagedONTs[fullID]
        GPONS["GPON %s/%s/%s" % (ont.frame, ont.slot, ont.gpon)] = (ont.frame, ont.slot, ont.gpon)
        print "%s" % (ont)

    # OjO: Para buscar las ID nuevas, se debe hacer la busqueda en la lista de administradas usando
    # los datos F/S/P de las no administradas.
    for GPON in GPONS.values() :
        ID = managedONTs.getNextFreeID(GPON[0], GPON[1], GPON[2])
        if ID == ONTSID_INVALID :
            print "El gpon %s/%s/%s no tiene posibilidad de tener mas ONTs" % (GPON[0], GPON[1], GPON[2])
        else:
            print "Siguiente ID libre para %s/%s/%s: %s" % (GPON[0], GPON[1], GPON[2], ID)
    return managedONTs

if __name__ == '__main__':
    test()
