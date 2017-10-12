#include "QOLTCommands.h"

namespace OLTCommands
{
OLTCommands::QOLTCommands::QOLTCommands()
{
	connect(this, SIGNAL(newResponse(QString,QString,QString)), this, SLOT(onCommandReceived(QString,QString,QString)));
}

void OLTCommands::QOLTCommands::setAdminMode()
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
	addCommand("SetScroll", oltConstants.setScroll(lines));
}

void QOLTCommands::getBoardInfo(quint8 frame, quint8 slot)
{
	addCommand("GetBoardInfo", oltConstants.getBoardInfo(frame, slot));
}

void QOLTCommands::getUnmanaged()
{
	addCommand("GetUnmanaged", oltConstants.getUnmanaged());
}

void QOLTCommands::getONTWANInfo(int frame, int slot, int port, int ontid)
{
	addCommand("GetONTWANInfo", oltConstants.getONTWANInfo(frame, slot, port, ontid));
}

void QOLTCommands::getONTMACInfo(int frame, int slot, int port, int ontid)
{
	addCommand("GetONTMACInfo", oltConstants.getONTMACInfo(frame, slot, port, ontid));
}

void QOLTCommands::getONTVersion(int frame, int slot, int port, int ontid)
{
	addCommand( "GetONTVersion", oltConstants.getONTVersion(frame, slot, port, ontid) );
}

void QOLTCommands::getGPONServiceProfiles()
{
	addCommand( "GetGPONServiceProfiles", oltConstants.getGPONServiceProfiles() );
}

void QOLTCommands::getGPONServiceProfile(int serviceProfileID)
{
	addCommand( "GetGPONServiceProfile", oltConstants.getGPONServiceProfile(serviceProfileID) );
}

void QOLTCommands::enterGPONSrvcPrfl(int serviceProfileID)
{
	addCommand( "EnterGPONSrvPrfl", oltConstants.enterGPONSrvProfile(serviceProfileID) );
}

void QOLTCommands::getTrafficTableIPs()
{
	addCommand( "GetTrafficTableIPs", oltConstants.getTrafficTableIPs() );
}

void QOLTCommands::getTrafficTableIP(int index)
{
	addCommand( "GetTrafficTableIP", oltConstants.getTrafficTableIP(index) );
}

void QOLTCommands::addTrafficTableIP(const QString &name, int cir, int pir, int priority)
{
	addCommand( "AddTrafficTableIP", oltConstants.addTrafficTableIP(name, cir, pir, priority) );
}

void QOLTCommands::modTrafficTableIP(int index, const QString &name, int cir, int pir, int priority)
{
	addCommand( "ModTrafficTableIP", oltConstants.modTrafficTableIP(index, name, cir, pir, priority) );
}

void QOLTCommands::delTrafficTableIP(int index)
{
	addCommand( "DelTrafficTableIP", oltConstants.delTrafficTableIP(index) );
}

void QOLTCommands::getDBAPProfiles()
{
	addCommand( "GetDBAProfiles", oltConstants.getDBAProfiles() );
}

void QOLTCommands::getDBAPProfile(int index)
{
	addCommand( "GetDBAProfile", oltConstants.getDBAProfile(index) );
}

void QOLTCommands::addDBAPProfile(const QString &name, const QString &type, const QString &speeds)
{
	addCommand( "AddDBAProfile", oltConstants.addDBAProfile(name, type, speeds) );
}

void QOLTCommands::delDBAPProfile(int index)
{
	addCommand( "DelDBAProfile", oltConstants.delDBAProfile(index) );
}

void QOLTCommands::getServicePorts(int frame, int slot, int port, int ontid)
{
	addCommand( "GetServicePorts", oltConstants.getServicePorts(frame, slot, port, ontid) );
}

void QOLTCommands::getServicePort(int frame, int slot, int port, int ontid, int index)
{
//	addCommand( "GetServicePorts", oltConstants.getServicePort(frame, slot, port, ontid, index) );
}

void QOLTCommands::addServicePort(int frame, int slot, int port, int ontid, int oltVLAN, int ontVLAN, int gemport, int upTrafficIndex, int dwTrafficIndex)
{
	if( (upTrafficIndex != -1) && (dwTrafficIndex != -1) )
		addCommand( "AddServicePort", oltConstants.addServicePortLimited(oltVLAN, frame, slot, port, ontid, gemport, ontVLAN, upTrafficIndex, dwTrafficIndex) );
	else
		addCommand( "AddServicePort", oltConstants.addServicePortUnlimited(oltVLAN, frame, slot, port, ontid, gemport, ontVLAN) );
}

void QOLTCommands::delServicePort(int index)
{
	addCommand( "DelServicePort", oltConstants.delServicePort(index) );
}

void QOLTCommands::getLineProfiles(int gepon)
{
	addCommand( "GetLineProfiles", oltConstants.getLineProfiles(gepon) );
}

void QOLTCommands::getLineProfile(int gepon, int index, const QString &name)
{
	addCommand( "GetLineProfile", oltConstants.getLineProfile(gepon, index, name) );
}

void QOLTCommands::addLineProfile(int gepon, const QString &name)
{
	addCommand( "AddLineProfile", oltConstants.addLineProfile(gepon, name) );
}

void QOLTCommands::delLineProfile(int gepon, int index)
{
	addCommand( "DelLineProfile", oltConstants.delLineProfile(gepon, index) );
}

void QOLTCommands::onCommandReceived(const QString &label, const QString &cmd, const QString data)
{
	if( label == "GetBoardInfo" )				emit boardInfo( BoardInfo(label, cmd, data) );
	else if( label == "GetUnmanaged" )			emit unmanagedOnts( UnmanagedONTs(label, cmd, data) );
	else if( label == "GetONTWANInfo" )			emit ontWANInfo( ONTWANInfo(label, cmd, data) );
	else if( label == "GetONTMACInfo" )			emit ontMACInfo( ONTMACInfo(label, cmd, data) );
	else if( label == "GetONTVersion" )			emit ontVersionInfo( ONTVersion(label, cmd, data) );
	else if( label == "GetGPONServiceProfiles" )emit gponServiceProfiles( GPONServiceProfiles(label, cmd, data) );
	else if( label == "GetGPONServiceProfile" )	emit gponServiceProfile( GPONServiceProfile(label, cmd, data) );

	else if( label == "GetTrafficTableIPs" )	emit trafficTableIPs( TrafficTableIPs(label, cmd, data) );
	else if( label == "GetTrafficTableIP" )		emit trafficTableIP( TrafficTableIP(label, cmd, data) );
	else if( label == "AddTrafficTableIP" )		emit trafficTableIP( TrafficTableIP(label, cmd, data) );
	else if( label == "ModTrafficTableIP" )		emit trafficTableIP( TrafficTableIP(label, cmd, data) );
	else if( label == "DelTrafficTableIP" )		emit trafficTableIP( TrafficTableIP(label, cmd, data) );

	else if( label == "GetDBAProfiles" )	emit trafficTableIPs( TrafficTableIPs(label, cmd, data) );
	else if( label == "GetTrafficTableIP" )		emit trafficTableIP( TrafficTableIP(label, cmd, data) );
	else if( label == "AddTrafficTableIP" )		emit trafficTableIP( TrafficTableIP(label, cmd, data) );
	else if( label == "ModTrafficTableIP" )		emit trafficTableIP( TrafficTableIP(label, cmd, data) );
	else if( label == "DelTrafficTableIP" )		emit trafficTableIP( TrafficTableIP(label, cmd, data) );
}
}
