#include "QOLTCommands.h"

QOLTCommands::QOLTCommands()
{
	connect(this, SIGNAL(newResponse(QString,QString,QString)), this, SLOT(onCommandReceived(QString,QString,QString)));
	connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnected()) );
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

void QOLTCommands::getDBAProfile(int id)
{
	addCommand( "GetDBAProfile", oltConstants.getDBAProfile(id) );
}

void QOLTCommands::addDBAProfile(const QString &name, int type, int fix, int assured, int max, bool compensate)
{
	addCommand( "AddDBAProfile", oltConstants.addDBAProfile(name, type, fix, assured, max, compensate) );
}

void QOLTCommands::modDBAProfile(int id, const QString &name, int type, int fix, int assured, int max, bool compensate)
{
	addAutoResponce( oltConstants.constant_ModDBAProfile_NewType(), QString("%1\n").arg(type) );
	switch( type )
	{
	case 1:
		addAutoResponce( oltConstants.constant_ModDBAProfile_NewFix(), QString("%1\n").arg(fix) );
		addAutoResponce( oltConstants.constant_ModDBAProfile_NewCompensation(), compensate ? "1\n" : "0\n" );
		break;
	case 2:
		addAutoResponce( oltConstants.constant_ModDBAProfile_NewAssure(), QString("%1\n").arg(assured) );
		break;
	case 3:
		addAutoResponce( oltConstants.constant_ModDBAProfile_NewAssure(), QString("%1\n").arg(assured) );
		addAutoResponce( oltConstants.constant_ModDBAProfile_NewMax(), QString("%1\n").arg(max) );
		break;
	case 4:
		addAutoResponce( oltConstants.constant_ModDBAProfile_NewMax(), QString("%1\n").arg(max) );
		break;
	case 5:
		addAutoResponce( oltConstants.constant_ModDBAProfile_NewFix(), QString("%1\n").arg(fix) );
		addAutoResponce( oltConstants.constant_ModDBAProfile_NewAssure(), QString("%1\n").arg(assured) );
		addAutoResponce( oltConstants.constant_ModDBAProfile_NewMax(), QString("%1\n").arg(max) );
		break;
	}
	addCommand( "ModDBAProfile", oltConstants.modDBAProfile(id, name) );
}

void QOLTCommands::delDBAProfile(int id)
{
	addCommand( "DelDBAProfile", oltConstants.delDBAProfile(id) );
}

void QOLTCommands::getServicePorts()
{
	addCommand( "GetServicePorts", oltConstants.getServicePorts() );
}
void QOLTCommands::getServicePort(int index)
{
	addCommand( "GetServicePort", oltConstants.getServicePort(index) );
}
void QOLTCommands::getONTServicePort(int frame, int slot, int port, int ontid)
{
	addCommand( "GetONTServicePort", oltConstants.getONTServicePort(frame, slot, port, ontid) );
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

void QOLTCommands::getGPONLineProfiles()
{
	addCommand( "GetGPONLineProfiles", oltConstants.getGPONLineProfiles() );
}
void QOLTCommands::getGPONLineProfile(int id)
{
	addCommand( "GetGPONLineProfile", oltConstants.getGPONLineProfile(id) );
}
void QOLTCommands::addGPONLineProfile(const QString &name)
{
	addCommand( "AddGPONLineProfile", oltConstants.addGPONLineProfile(name) );
}
void QOLTCommands::delGPONLineProfile(int id)
{
	addCommand( "DelGPONLineProfile", oltConstants.delGPONLineProfile(id) );
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

	else if( label == "GetGPONLineProfiles" )	emit lineProfiles( OLTCommands::LineProfiles(label, cmd, data) );
	else if( label == "GetGPONLineProfile" )	emit lineProfile( OLTCommands::LineProfile(label, cmd, data) );
	else if( label == "AddGPONLineProfile" )	emit lineProfile( OLTCommands::LineProfile(label, cmd, data) );
	else if( label == "ModGPONLineProfile" )	emit lineProfile( OLTCommands::LineProfile(label, cmd, data) );
	else if( label == "DelGPONLineProfile" )	emit lineProfile( OLTCommands::LineProfile(label, cmd, data) );

	else if( label == "GetServicePorts" )		emit servicePorts( OLTCommands::ServicePorts(label, cmd, data) );
	else if( label == "GetServicePort" )		emit servicePort( OLTCommands::ServicePort(label, cmd, data) );
	else if( label == "GetONTServicePort" )		emit servicePort( OLTCommands::ServicePort(label, cmd, data) );
	else if( label == "AddServicePort" )		emit servicePort( OLTCommands::ServicePort(label, cmd, data) );
	else if( label == "ModServicePort" )		emit servicePort( OLTCommands::ServicePort(label, cmd, data) );
	else if( label == "DelServicePort" )		emit servicePort( OLTCommands::ServicePort(label, cmd, data) );
}

void QOLTCommands::onDisconnected()
{
}

bool QSpeedSpinBox::event(QEvent *e)
{
	if( e->type() == QEvent::FocusOut )
	{
		// Round to 64.
		setValue(value() & 0xFFFFFFC0);
	}
	return QSpinBox::event(e);
}


void QTypeSpinBox::onValueChanged(int type)
{
	if( m_fix && m_assured && m_max )
	{
		if( isEnabled() )
		{
			/*
			 * type1                 Fixed bandwidth
			 * type2                 Assured bandwidth
			 * type3                 Assured bandwidth, Maximum bandwidth
			 * type4                 Maximum bandwidth
			 * type5                 Fixed bandwidth, Assured bandwidth, Maximum bandwidth
			 */
			switch( type )
			{
			case 1: m_fix->setEnabled(true);	m_assured->setEnabled(false);	m_max->setEnabled(false); m_compensate->setEnabled(true);	break;
			case 2: m_fix->setEnabled(false);	m_assured->setEnabled(true);	m_max->setEnabled(false); m_compensate->setEnabled(false);	break;
			case 3: m_fix->setEnabled(false);	m_assured->setEnabled(true);	m_max->setEnabled(true); m_compensate->setEnabled(false);	break;
			case 4: m_fix->setEnabled(false);	m_assured->setEnabled(false);	m_max->setEnabled(true); m_compensate->setEnabled(false);	break;
			case 5: m_fix->setEnabled(true);	m_assured->setEnabled(true);	m_max->setEnabled(true); m_compensate->setEnabled(false);	break;
			}
		}
		else
		{
			m_fix->setEnabled(false);
			m_assured->setEnabled(false);
			m_max->setEnabled(false);
		}
	}
}

bool QTypeSpinBox::event(QEvent *e)
{
	if( e->type() == QEvent::EnabledChange )
		onValueChanged(value());
	return QSpinBox::event(e);
}
