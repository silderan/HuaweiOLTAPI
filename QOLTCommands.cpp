#include "QOLTCommands.h"

QOLTCommands::QOLTCommands()
{
	connect(this, SIGNAL(newResponse(QString,QString,QString)), this, SLOT(onCommandReceived(QString,QString,QString)));
}

void QOLTCommands::setAdminMode()
{
	if( oltState() < QTelnetInterface::OltAdminMode )
		addCommand( "EnablingAdmin", oltConstants.commandEnableAdmin(), OltAdminMode );
}

void QOLTCommands::setConfigMode()
{
	if( oltState() < QTelnetInterface::OltConfigMode )
	{
		setAdminMode();
		addCommand( "ConfigMode", oltConstants.commandEnterConfigMode(), OltConfigMode );
	}
}

void QOLTCommands::quitContext()
{
	addCommand("QuittingContext", oltConstants.quitContext());
}

void QOLTCommands::setScroll(int lines)
{
	addCommand("Scroll", oltConstants.scroll(lines));
}

void QOLTCommands::getBoardInfo(quint8 frame, quint8 slot)
{
	addCommand("BoardInfo", oltConstants.boardInfo(frame, slot));
}

void QOLTCommands::getUnmanaged()
{
	addCommand("GetUnmanaged", oltConstants.unmanaged());
}

void QOLTCommands::getONTWANInfo(int frame, int slot, int port, int ontid)
{
	addCommand("ONTWANInfo", oltConstants.ontWANInfo(frame, slot, port, ontid));
}

void QOLTCommands::getONTMACInfo(int frame, int slot, int port, int ontid)
{
	addCommand("ONTMACInfo", oltConstants.ontMACInfo(frame, slot, port, ontid));
}

void QOLTCommands::getGPONServiceProfiles()
{
	addCommand( "GPONServiceProfiles", oltConstants.gponServiceProfiles() );
}

void QOLTCommands::getGPONServiceProfile(int serviceProfileID)
{
	addCommand( "GPONServiceProfile", oltConstants.gponServiceProfile(serviceProfileID) );
}

void QOLTCommands::enterGPONSrvcPrfl(int serviceProfileID)
{
	addCommand( "EnterGPONSrvPrfl", oltConstants.enterGPONSrvProfile(serviceProfileID) );
}

void QOLTCommands::getONTVersion(int frame, int slot, int port, int ontid)
{
	addCommand( "ONTVersion", oltConstants.ontVersion(frame, slot, port, ontid) );
}

void QOLTCommands::onCommandReceived(const QString &label, const QString &cmd, const QString data)
{
	if( label == "BoardInfo" )					emit boardInfo( BoardInfo(label, cmd, data) );
	else if( label == "GetUnmanaged" )			emit unmanagedOnts( UnmanagedONTs(label, cmd, data) );
	else if( label == "ONTWANInfo" )			emit ontWANInfo( ONTWANInfo(label, cmd, data) );
	else if( label == "ONTMACInfo" )			emit ontMACInfo( ONTMACInfo(label, cmd, data) );
	else if( label == "ONTVersion" )			emit ontVersionInfo( ONTVersion(label, cmd, data) );
	else if( label == "GPONServiceProfiles" )	emit gponServiceProfiles( GPONServiceProfiles(label, cmd, data) );
	else if( label == "GPONServiceProfile" )	emit gponServiceProfile( GPONServiceProfile(label, cmd, data) );
}
