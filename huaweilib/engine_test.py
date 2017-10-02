#!/usr/bin/env python
from engine import *
import time
from datetime import timedelta

def speed_profiles_compare(spA, spB):
	return isinstance(spB, SpeedProfile) and isinstance(spA, SpeedProfile) \
		 and spA.name == spB.name \
		 and spA.uploadTotal == spB.uploadTotal \
		 and spA.uploadAssured == spB.uploadAssured \
		 and spA.uploadPriority == spB.uploadPriority \
		 and spA.downloadTotal == spB.downloadTotal \
		 and spA.downloadAssured == spB.downloadAssured \
		 and spA.downloadPriority == spB.downloadPriority

def test_speed_profiles(ontsManager) :
	inicia = time.time()
	print "Test de perfiles iniciado el %s" % (time.ctime(inicia))
	pp = ontsManager.get_speed_profiles()
	if pp :
		print "Mostrando %d perfil(es) encontrados:" % (len(pp))
		for p in pp.values() :
			print p.__str__()
	else:
		print "Ningun perfil de velocidad"

	spA = SpeedProfile()
	spA.name = "PerfilDe-.Prueba"
	spA.downloadAssured = "2048"
	spA.downloadTotal = "4096"
	spA.downloadPriority = "2"
	spA.uploadAssured = "4096"
	spA.uploadTotal = "8192"
	spA.uploadPriority = "2"
	ontsManager.setSpeedProfile(spA)
	spB = ontsManager.get_speed_profile(spA.name)
	assert speed_profiles_compare(spA, spB), "El perfil de velocidad anyadido no se ha localizado en la lista"
	print "Perfil de velocidad anyadido correctamente"

	ontsManager.delSpeedProfile(speedProfileName=spA.name)
	spB = ontsManager.get_speed_profile(spA.name)
	assert spB == None, "El perfil de velocidad no se ha borrado de la lista"
	print "Perfil de velocidad borrado correctamente"

	ontsManager.setSpeedProfile(spA)
	spB = ontsManager.get_speed_profile(spA.name)
	assert speed_profiles_compare(spA, spB), "El perfil de velocidad anyadido de nuevo no se ha localizado en la lista"
	print "Perfil de velocidad anyadido de nuevo correctamente"

	spA.id = spB.id	# para que se modifique en vez de crear uno de nuevo.
	spA.name = "PerfilPrueba2"
	spA.downloadAssured = "8192"
	spA.downloadTotal = "16384"
	spA.uploadAssured = "8192"
	spA.uploadTotal = "16384"
	ontsManager.setSpeedProfile(spA)
	spB = ontsManager.get_speed_profile(spB.name)
	assert spB == None, "No ha desaparecido el perfil de velocidad con el nombre 'viejo' al modificarlo o no se ha modificado"
	spB = ontsManager.get_speed_profile(spA.name)
	assert speed_profiles_compare(spA, spB), "Los perfiles no coinciden despues de intentar modificarlo"
	ontsManager.delSpeedProfile(speedProfileName=spA.name)
	print "Modificacion de perfiles correcta"
	print "Se ha completado el test de perfiles en %s" % (timedelta(seconds=int(time.time()-inicia)))

def _add_del_dba(ontsManager, dba) :
	if ontsManager.addDBAProfile(dba) :
		print "DBA %s anyadido con ID %s" % (dba.name, dba.profile_id)
		if ontsManager.delDBAProfile(dba.profile_id) :
			print "DBA %s borrado" % (dba.profile_id)
		else :
			raise Exception("No se ha podido borrar el DBA %s con id %s " % (dba.name, dba.profile_id) )
	else :
		print "No se ha podido anyadir el DBA %s con id %s " % (dba.name, dba.profile_id)

def _add_del_fake_dba(ontsManager, dba) :
	try: 
		_add_del_dba(ontsManager, dba)
		raise Exception("El DBA Falso %s se ha anyadido y borrado y no deberia")
	except DBAException as e:
		assert not dba.profile_id or len(dba.profile_id) == 0, "El DBA Falso %s con id %s se ha anyadido pero no borrado" % (dba.name, dba.profile_id)
		print "Error al anyadir el DBA falso.. CORRECTO! %s" % (e)
	except:
		raise

def test_dba_profiles(ontsManager) :
	inicia = time.time()
	print "Test de dba-profiles iniciado el %s" % (time.ctime(inicia))

#	dbas = ontsManager.getDBAProfiles()
#
#	if dbas :
#		print "Mostrando %d perfil(es) encontrados:" % (len(dbas))
#		for p in dbas.values() :
#			print p.__str__()
#	else:
#		print "Ningun perfil DBA"

	dba = DBAProfile()
	dba.name = "Tipo 1: DBA Fijo"
	dba.bw_fix = "128"
	dba.bw_compensation = "yes"
	_add_del_dba(ontsManager, dba)

	dba = DBAProfile()
	dba.name = "Tipo 2: DBA Asegurado"
	dba.bw_assure = "256"
	_add_del_dba(ontsManager, dba)

	dba = DBAProfile()
	dba.name = "Tipo 3: DBA Asegurado y maximo"
	dba.bw_assure = "256"
	dba.bw_max = "512"
	_add_del_dba(ontsManager, dba)

	dba = DBAProfile()
	dba.name = "Tipo 4: DBA Maximo"
	dba.bw_max = "512"
	_add_del_dba(ontsManager, dba)

	dba = DBAProfile()
	dba.name = "Tipo 5: DBA multi tipo"
	dba.bw_fix = "128"
	dba.bw_assure = "256"
	dba.bw_max = "512"
	_add_del_dba(ontsManager, dba)

	dba = DBAProfile()
	dba.name = "Tipo x: no valido"
	dba.bw_fix = "128"
	dba.bw_assure = "256"
	_add_del_fake_dba(ontsManager, dba)

	dba = DBAProfile()
	dba.name = "Tipo 3: no valido"
	dba.bw_assure = "300"
	dba.bw_max = "256"
	_add_del_fake_dba(ontsManager, dba)

	dba = DBAProfile()
	dba.name = "Tipo 5: no valido"
	dba.bw_fix = "128"
	dba.bw_assure = "300"
	dba.bw_max = "256"
	_add_del_fake_dba(ontsManager, dba)

	dba = DBAProfile()
	dba.name = "Tipo 5: no valido"
	dba.bw_fix = "300"
	dba.bw_assure = "256"
	dba.bw_max = "512"
	_add_del_fake_dba(ontsManager, dba)

	print "Se ha completado el test de dba-profiles en %s" % (timedelta(seconds=int(time.time()-inicia)))

def other_test(ontsManager):
#    ont = ONTInfo()
#    ont.SN = "1234567890ABCDEFG"
#    createAllDefaultWiFi(ont)
#    print ont.wifi.networks[0].ssid
#    print ont.wifi.networks[0].wpa
#    return

	print "\nONTs administradas"
	managed = ontsManager.get_managed(0, 0)
	if not managed :
		print "Ninguna ONT administrada"
	else :
		for ont in managed :
			print "%s" % (ont)
			if ont.isOnline() :
				print ontsManager.get_ont_wan_info(ont.frame, ont.slot, ont.gpon, ont.ID)

	# Saco la informacion de las ONTs nuevas
	# en este caso, el key es el Serial Number de las ONT
	print "\nONTs nuevas"
	unmanaged = ontsManager.get_unmanaged()
	if not unmanaged :
		print "Ninguna ONT nueva"
	else :
		for ont in unmanaged:
			print "%s" % (ont)

if __name__ == '__main__':
	import sys
	inicia = time.time()

	allFncs = { "speed" : ("Comprueba los traffic-table-ip", "test_speed_profiles"),\
							"service" : ("Comprueba los srv-profile", "test_srv_profiles"),\
							"dba" : ("Comprueba los dba-profile", "test_dba_profiles"),\
							"other" : ("Ejecuta tests no clasificados", "other_test"),\
							"all" : ("Ejecuta todos los tests", ""),\
							"?" : ("Muestra la ayuda del comando", "") }

	fncs = list()

	for arg in sys.argv :
		if arg == "all" :
			fncs = map( lambda k: k[1], allFncs.values() )
			break
		if arg == "?" :
			fncs = list()
			break
		elif allFncs.has_key(arg) :
			fncs += (allFncs[arg][1],)

	print fncs
	if not len(fncs) :
		print "Opciones para comprobar la libreria"
		for f in allFncs.keys() :
			print "%20s : %s" % (f, allFncs[f][0])
	else :
		print "Test iniciado el %s" % (time.ctime(inicia))
		ontsManager = OntsManager("10.50.0.3", "root", "admin")
		ontsManager.connectOLT()
		for fnc in fncs :
			globals()[fnc](ontsManager)
		ontsManager.disconnectOLT()
		print "Se ha completado el test en %s" % (timedelta(seconds=int(time.time()-inicia)))
