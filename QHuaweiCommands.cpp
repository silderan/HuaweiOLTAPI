#include "QHuaweiCommands.h"


QHuaweiCommands::QHuaweiCommands()
{
//	m_comandErrors.addErrorString( "Unknown command", tr("Comando desconocido o no se esta en el contexto correcto") );
//	m_comandErrors.addErrorString( "Failure", tr("Fallo al ejecutar el comando") );
//	m_comandErrors.addErrorString( "Parameter error", tr("Parametro del comando erroneo mal escrito o con caracteres no validos") );
//	m_comandErrors.addErrorString( "Incomplete command", tr("El comando esta incompleto. Faltan parametros o son erroneos") );
//	m_comandErrors.addErrorString( "Too many parameters", tr("Demasiados argumentos en el comando") );
}

void QHuaweiCommands::enableAdminMode()
{
	if( oltState() < QTelnetInterface::OltAdminMode )
		addCommand( "EnablingAdmin", "enable", "\nMA5683T#", OltLogging );
}

void QHuaweiCommands::enterConfigMode()
{
	enableAdminMode();
	if( oltState() < QTelnetInterface::OltConfigMode )
		addCommand( "ConfigMode", "config", "\nMA5683T(config)#", OltLogged );
}
