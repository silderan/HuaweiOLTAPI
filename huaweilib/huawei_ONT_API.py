#!/usr/bin/env python
# -*- coding: latin-1 -*-
import re
import struct
from telnetlib import Telnet
from array import array
from time import time as currentTime
from logger import Logger
from huawei_cnfg import *
from huawei_cmds import *
from data_classes import *

PROMPT = "WAP>"

class OntManager(object) :
	def __init__(self, ipaddr, username, password, port=10023):
		self.ipaddr = ipaddr
		self.username = username
		self.password = password
		self.port = port
		self.ontConn = 0
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
		if not self.ontConn :
			return False
		#TODO: Comprobar que la conexion se mantiene activa.
		return True

	def _connect_lazy(self) :
		HuaweiAPIError.check( self.username, "Falta el nombre de usuario" )
		HuaweiAPIError.check( self.ipaddr, "Falta la direccion IP de la ONT" )
		HuaweiAPIError.check( self.password, "Falta la contrasenya de usuario" )
		HuaweiAPIError.check( self.port, "Falta el puerto de conexion a la ONT" )
		self.logger.info( "Conectando a %s@%s:%s" % (self.username, self.ipaddr, self.port) )
		self.ontConn = Telnet(self.ipaddr, self.port, LOGIN_TIMEOUT)
		HuaweiAPIError.check( self.ontConn, "No se ha podido conectar con la ONT" )
		self.logger.info( "Conectado a %s@%s:%s" % (self.username, self.ipaddr, self.port) )

		out = self.ontConn.read_until( "Login", LOGIN_TIMEOUT )
		HuaweiAPIError.check( out.find( "login" ) < 0, ASSERT_BAD_USER_ASK_TEXT )
		self.ontConn.write( buffer(self.username+"\n") )

		out = self.ontConn.read_until( "Password", LOGIN_TIMEOUT )
		HuaweiAPIError.check( out.find( "Password" ) >= 0, ASSERT_BAD_PASS_ASK_TEXT )
		self.ontConn.write( buffer(self.password+"\n") )

		#Espero el primer prompt
		out = self.ontConn.read_until( PROMPT, CMD_TIMEOUT)
		HuaweiAPIError.check( out.find( PROMPT ) >= 0, BAD_LOGIN )
		self.logger.info("Logado a %s@%s:%s" % (self.username, self.ipaddr, self.port) )

	def connect(self):
		if (not self.connecting) and (not self.isConnected()) :
			self.connecting = True
			try :
				self._connect_lazy()
			except :
				self.disconnect()
				raise
			self.connecting = False
		return None

	def disconnect(self):
		if self.isConnected() :
			try:
				self.ontConn.close()
			except:
				raise
			finally:
				self.logger.info("Desconectado de %s@%s:%s" % (self.username, self.ipaddr, self.port) )
				self.ontConn = 0
				self.connecting = False
				self.sending_cmd = ""

	def _send_cmd_lazy(self, prompt) :
		self.logger.logit( "SendCmd", self.sending_cmd )
		self.ontConn.write( buffer("%s\n" % self.sending_cmd) )

		out = self.ontConn.read_until(prompt, CMD_TIMEOUT)
		if out.find(prompt) < 0 :
			raise HuaweiAPIError("No se ha recibido la respuesta completa desde la ONT")
		HuaweiAPIError.check( out, "La ONT no ha retornado ningun texto. Quizá se ha perdido la conexion" )

		return out

	def _send_cmd(self, cmd, prompt=PROMPT):
		HuaweiAPIError.check( self.connecting == False, "Aun no se ha completado la conexion con la ONT y se intenta enviar el comando '%s'!" % (cmd) )
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
		HuaweiAPIError.check( self.isConnected(), "No esta conectado" )
		if cmdParser :
			_cmd = self._parse_cmd(_cmd, cmdParser)
		return self._send_cmd(_cmd)

	def get_optical_info(self) :
		out = self.send_cmd( "display optic")
		data = {}
		i = 0
		for line in out.splitlines() :
			pair = line.split(":")
			if len(pair) == 2 :
				pair[0] = pair[0].strip()
				pair[1] = pair[1].strip().split(" ")[0]
				try:
					data[pair[0]] = int(pair[1])
				except:
					try:
						data[pair[0]] = float(pair[1])
					except:
						pass
		return data

	def get_serial_number(self) :
		out = self.send_cmd("display sn")
		match = re.search(" = ([0-9A-Z]+)", out)
		if match  :
			return match.group(1)
		return ""

class DataFile(object) :
	def __init__(self, fname) :
		self.fname = fname
	def write_int(integer) :
		with open(self.fname) as f :
			f.write

NUMBER_OF_SAMPLES = 60

class ONTInfo(OntManager) :
	def __init__(self, ipaddr, username, password, port):
		self.curVersion = 1	# Versión del fichero
		self.SN = None			# Número de serie de la ONT (se usa para crear el nombre del fichero)
		self.optical = None		# Lista de datos de la potencia óptica
		self.latency = 0			# Lista de latencias.
		self.lastTime = 0			# Cuando se actualizaron los datos por última vez.
		super(ONTInfo, self).__init__(ipaddr, username, password, port)

	def __str__(self) :
		if self.optical and self.latency :
			i = 0
			dat = ""
			while i < NUMBER_OF_SAMPLES :
				dat += "(%s,%s)" % (self.optical[i], self.latency[i])
				i+=1
		else:
			dat = "No data available"
		return 	"CurVersion: %s\n"\
						"SN        : %s\n"\
						"lastTime: : %s\n"\
						"Size      : %i\n"\
						"Optical   : %s"\
						% (self.curVersion, self.SN, self.lastTime, NUMBER_OF_SAMPLES, dat)

	def _get_fname(self) :
		return "%s.data" % self.SN

	def _load(self) :
		with open(self._get_fname(), 'rb') as fin :
			# Primero la versión del fichero.
			fileVersion = struct.unpack('i', fin.read(4))[0]
			if fileVersion == 1 :
				# Segundo el time-stamp.
				self.lastTime = struct.unpack('i', fin.read(4))[0]
				# Tercero la cantidad de datos.
				cant = struct.unpack('i', fin.read(4))[0]
				if cant :
					# Cuarto y quinto los datos de la señal óptica y latencias.
					self.optical = array('h')
					self.optical.fromfile(fin, cant)
					self.latency = array('h')
					self.latency.fromfile(fin, cant)
				else :
					self.optical = None
					self.latency = None

	def load(self) :
		if not self.SN :
			self.SN = self.get_serial_number()
			HuaweiAPIError.check( self.SN, "No se pueden cargar los datos de la ONT porque no se sabe el numero de serie" )
		try :
			self._load()
		except Exception as e:
			logger.error("No se ha podido cargar el fichero %s: %s" % (self._get_fname(), e) )
			return False
		return True

	def save(self) :
		with open(self._get_fname(), 'wb') as fout:
			# Primero la versión.
			fout.write(struct.pack('i', int(self.curVersion)))
			# Segundo el time-stamp.
			fout.write(struct.pack('i', int(self.lastTime)))
			# Tercero la cantidad de datos.
			if self.optical and len(self.optical) :
				fout.write(struct.pack('i', len(self.optical)))
				# Cuarto y quinto los datos de la señal óptica y latencias.
				self.optical.tofile(fout)
				self.latency.tofile(fout)
			else :
				fout.write(struct.pack('i', 0))

	# Borra los datos en el rango de tiempo indicado.
	def _clearRangeData(self, firstTime, lastTime) :
		# Si el rango a borrar es tan alto que debería borrar todos los datos, borro la lista entera.
		if firstTime + NUMBER_OF_SAMPLES < lastTime :
			self.logger.info("Latencia superior a %i; borrando listas" % NUMBER_OF_SAMPLES )
			self.optical = None
			self.latency = None
		else :
			firstTime = int(firstTime)
			lastTime = int(lastTime)
			self.logger.info("Latencia de %i segundo(s); borrando datos" % (lastTime - firstTime) )
			while firstTime <= lastTime :
				self.optical[firstTime % NUMBER_OF_SAMPLES] = 0
				self.latency[firstTime % NUMBER_OF_SAMPLES] = 0
				firstTime += 1
		return None

	def update_info(self) :
		if self.SN == None :
			self.SN = self.get_serial_number()
		if not self.optical :
			self.optical = array('h')
			self.optical.extend([0] * NUMBER_OF_SAMPLES)
		if not self.latency :
			self.latency = array('h')
			self.latency.extend([0] * NUMBER_OF_SAMPLES)
		oldTimeStamp = currentTime()
		#self.load()
		# Borramos el dato actual.
		self.optical[int(tm % NUMBER_OF_SAMPLES)] = 0
		self.latency[int(tm % NUMBER_OF_SAMPLES)] = 0

		try :
			newRxPower = self.get_optical_info()["RxPower"]
			newTimeStamp = currentTime()
			# Borro los posibles "huecos" que se puedan producir por una latencia alta.
			raise Exception("last=%i new=%i" % (self.lastTime, newTimeStamp))
			
			if self.lastTime and int(newTimeStamp) != int(self.lastTime+1) :
				self._clearRangeData(self.lastTime+1, newTimeStamp)

			self.lastTime = newTimeStamp
			indice = int(self.lastTime % NUMBER_OF_SAMPLES)
			# Guardo los datos float convertidos a un entero pero multiplicado por 256 para no perder muchos decimales
			try:
				self.latency[indice] = int((newTimeStamp-oldTimeStamp) * 256)
			except OverflowError:
				self.latency[indice] = int(32768 if newTimeStamp-oldTimeStamp > 0 else -32768)
			try:
				self.optical[indice] = int(newRxPower * 256)
			except OverflowError:
				self.optical[indice] = int(32768 if newRxPower > 0 else -32768)
			print "%i: %fdb %fseg." % (self.lastTime, float(self.optical[indice]) / 256.0, float(self.latency[indice])/256.0)
		except :
			newTimeStamp = currentTime()
			if self.lastTime and int(self.lastTime+1) != int(newTimeStamp) :
				self._clearRangeData( self.lastTime+1, newTimeStamp )
			raise

	def getPrintableData(self) :
		i = 0
		rtn = [None] * NUMBER_OF_SAMPLES
		while i < NUMBER_OF_SAMPLES :
			fecha = (int(self.lastTime) - NUMBER_OF_SAMPLES) + i
			indice = fecha % NUMBER_OF_SAMPLES
			latencia = self.latency[indice]
			potencia = self.optical[indice]
			rtn[i] = [fecha, potencia, latencia]
			i += 1
		return rtn

if __name__ == '__main__':

	import sys
	ontInfo = ONTInfo("10.50.219.95", "root", "admin", 10023)

	ontInfo.connect()
	ontInfo.load()
	tm = 0
	i = 0
	while i < 65 :
		while tm == int(currentTime()) :
			pass
		tm = int(currentTime())
		ontInfo.update_info()
		i+=1
	print ontInfo
