#include "QOLTCommands.h"

QOLTCommands::QOLTCommands()
{
	connect(this, SIGNAL(newResponse(QString,QString,QString)), this, SLOT(onCommandReceived(QString,QString,QString)));
}

void QOLTCommands::setAdminMode()
{
	if( oltState() < QTelnetInterface::OltAdminMode )
		addCommand( "EnablingAdmin", oltConstants.commandEnableAdmin(), oltConstants.promptAdmin(), OltAdminMode );
}

void QOLTCommands::setConfigMode()
{
	if( oltState() < QTelnetInterface::OltConfigMode )
	{
		setAdminMode();
		addCommand( "ConfigMode", oltConstants.commandEnterConfigMode(), oltConstants.promptConfig(), OltConfigMode );
	}
}

void QOLTCommands::setScroll(int lines)
{
	addCommand("Scroll", oltConstants.scroll(lines), oltConstants.promptConfig());
}

void QOLTCommands::getBoardInfo(quint8 frame, quint8 slot)
{
	addCommand("BoardInfo", oltConstants.boardInfo(frame, slot), oltConstants.promptConfig());
}

void QOLTCommands::getUnmanaged()
{
	addCommand("GetUnmanaged", oltConstants.unmanaged(), oltConstants.promptConfig());
}

void QOLTCommands::getONTWANInfo(int frame, int slot, int port, int ontid)
{
	addCommand("ONTWANInfo", oltConstants.ontWANInfo(frame, slot, port, ontid), oltConstants.promptConfig());
}

void QOLTCommands::getONTMACInfo(int frame, int slot, int port, int ontid)
{
	addCommand("ONTMACInfo", oltConstants.ontMACInfo(frame, slot, port, ontid), oltConstants.promptConfig());
}

void QOLTCommands::getGPONServiceProfiles()
{
	addCommand( "GPONServiceProfiles", oltConstants.gponServiceProfiles(), oltConstants.promptConfig() );
}

void QOLTCommands::onCommandReceived(const QString &tag, const QString &cmd, const QString data)
{
	if( tag == "BoardInfo" )				emit boardInfo( BoardInfo(tag, cmd, data) );
	else if( tag == "GetUnmanaged" )		emit unmanagedOnts( UnmanagedONTs(tag, cmd, data) );
	else if( tag == "ONTWANInfo" )			emit ontWANInfo( ONTWANInfo(tag, cmd, data) );
	else if( tag == "ONTMACInfo" )			emit ontMACInfo( ONTMACInfo(tag, cmd, data) );
	else if( tag == "GPONServiceProfiles" )	emit gponServiceProfiles( GPONServiceProfiles(tag, cmd, data) );
}
