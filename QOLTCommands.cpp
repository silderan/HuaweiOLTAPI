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

void QOLTCommands::getDBAProfiles()
{
	addCommand( "GetDBAProfiles", oltConstants.getDBAProfiles() );
}

void QOLTCommands::getDBAProfile(int index)
{
	addCommand( "GetDBAProfile", oltConstants.getDBAProfile(index) );
}

void QOLTCommands::addDBAProfile(const QString &name, const QString &type, const QString &speeds)
{
	addCommand( "AddDBAProfile", oltConstants.addDBAProfile(name, type, speeds) );
}

void QOLTCommands::delDBAProfile(int index)
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
	if( label == "GetBoardInfo" )				emit boardInfo( OLTCommands::BoardInfo(label, cmd, data) );
	else if( label == "GetUnmanaged" )			emit unmanagedOnts( OLTCommands::UnmanagedONTs(label, cmd, data) );
	else if( label == "GetONTWANInfo" )			emit ontWANInfo( OLTCommands::ONTWANInfo(label, cmd, data) );
	else if( label == "GetONTMACInfo" )			emit ontMACInfo( OLTCommands::ONTMACInfo(label, cmd, data) );
	else if( label == "GetONTVersion" )			emit ontVersionInfo( OLTCommands::ONTVersion(label, cmd, data) );
	else if( label == "GetGPONServiceProfiles" )emit gponServiceProfiles( OLTCommands::GPONServiceProfiles(label, cmd, data) );
	else if( label == "GetGPONServiceProfile" )	emit gponServiceProfile( OLTCommands::GPONServiceProfile(label, cmd, data) );

	else if( label == "GetTrafficTableIPs" )	emit trafficTableIPs( OLTCommands::TrafficTableIPs(label, cmd, data) );
	else if( label == "GetTrafficTableIP" )		emit trafficTableIP( OLTCommands::TrafficTableIP(label, cmd, data) );
	else if( label == "AddTrafficTableIP" )		emit trafficTableIP( OLTCommands::TrafficTableIP(label, cmd, data) );
	else if( label == "ModTrafficTableIP" )		emit trafficTableIP( OLTCommands::TrafficTableIP(label, cmd, data) );
	else if( label == "DelTrafficTableIP" )		emit trafficTableIP( OLTCommands::TrafficTableIP(label, cmd, data) );

	else if( label == "GetDBAProfiles" )		emit dbaProfiles( OLTCommands::DBAProfiles(label, cmd, data) );
	else if( label == "GetDBAProfile" )			emit dbaProfile( OLTCommands::DBAProfile(label, cmd, data) );
	else if( label == "AddDBAProfile" )			emit dbaProfile( OLTCommands::DBAProfile(label, cmd, data) );
	else if( label == "ModDBAProfile" )			emit dbaProfile( OLTCommands::DBAProfile(label, cmd, data) );
	else if( label == "DelDBAProfile" )			emit dbaProfile( OLTCommands::DBAProfile(label, cmd, data) );

	else if( label == "GetLineProfiles" )		emit lineProfiles( OLTCommands::LineProfiles(label, cmd, data) );
	else if( label == "GetLineProfile" )		emit lineProfile( OLTCommands::LineProfile(label, cmd, data) );
	else if( label == "AddLineProfile" )		emit lineProfile( OLTCommands::LineProfile(label, cmd, data) );
	else if( label == "ModLineProfile" )		emit lineProfile( OLTCommands::LineProfile(label, cmd, data) );
	else if( label == "DelLineProfile" )		emit lineProfile( OLTCommands::LineProfile(label, cmd, data) );

	else if( label == "GetServicePorts" )		emit servicePorts( OLTCommands::ServicePorts(label, cmd, data) );
	else if( label == "GetServicePort" )		emit servicePort( OLTCommands::ServicePort(label, cmd, data) );
	else if( label == "AddServicePort" )		emit servicePort( OLTCommands::ServicePort(label, cmd, data) );
	else if( label == "ModServicePort" )		emit servicePort( OLTCommands::ServicePort(label, cmd, data) );
	else if( label == "DelServicePort" )		emit servicePort( OLTCommands::ServicePort(label, cmd, data) );
}
