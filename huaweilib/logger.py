# -*- coding: latin-1 -*-
import time
import sys

# Una clase básica y simple para registrar cambios (la que tiene python es demasiado compleja)
# TODO: Añadir la posibilidad de que se guarde en un fichero.
# TODO: Crear un "procesador" del texto del log para que retorne los datos en una estructura
#       y asi poderse mostrar "formateados" de la manera que el diseñador quiera.
class Logger(object):
	def __init__(self, tagsToLog=["Info", "Warning", "Error", "Critical", "Debug"], fname=""):
		assert isinstance(tagsToLog, list) or isinstance(tagsToLog, tuple), "Sólo se pueden pasar listas"
		self.tagsToLog = list(tagsToLog)
		self.fname = ""

	def set_log_file(self, fname):
		self.fname = fname;

	def add_tag(self, tag):
		assert isinstance(tag, str), "Sólo se pueden pasar cadenas"
		if tag not in self.tagsToLog :
			self.tagsToLog += [tag]

	def del_tag(self, tag):
		assert isinstance(tag, str), "Sólo se pueden pasar cadenas"
		if tag in self.tagsToLog :
			self.tagsToLog.remove(tag)

	def del_tags(self, tags):
		assert isinstance(self.tagsToLog, list) or isinstance(self.tagsToLog, tuple), "Sólo se pueden pasar listas"
		for tag in tags :
			self.del_tag(tag)

	def add_tags(self, tagsToLog):
		assert isinstance(self.tagsToLog, list) or isinstance(self.tagsToLog, tuple), "Sólo se pueden pasar listas"
		for tag in tags :
			self.add_tag(tag)

	def is_error(self, level) :
		return (level == "Error") or (level == "Critical")

	def logit(self, level="Info", msg=""):
		if len(msg) and (not level or (level in self.tagsToLog)) :
			out = "%s [%s] %s" % (time.ctime(), level, msg)
			if self.fname:
				# TODO: !!
				pass
			print >> sys.stderr, out

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
