#ifndef QOLTCONSTANTS_H
#define QOLTCONSTANTS_H

#include <QMap>
#include <QStringList>
#include <qstring.h>
#include <QRegExp>

class OLTConstants
{
public:
	class ConstantsBase  : public QMap<QString, QString>
	{
	public:
		void fromFileLine(const QByteArray &line);
	};

	class Constants : public ConstantsBase
	{
	};

	class Commands : public ConstantsBase
	{
	};

	class ErrorStrings : public ConstantsBase
	{
	public:
		QString errorString(const QString &text) const;
	};

private:
	Constants m_constants;
	QRegExp m_loginPrompt;
	QRegExp m_cmdPrompt;
	Commands m_oltCommands;
	ErrorStrings m_cmdErrors;
	ErrorStrings m_loginErrors;
	void loadOLTConstantsFile();

	QString parseCommand(QString command, const QStringList data) const;
public:
	OLTConstants();

	// Constants:
	QString constant_MoreText() const { return m_constants["MoreText"];	}
	QString constant_ModDBAProfile_NewType() const { return m_constants["ModDBAProfile_NewType"]; }
	QString constant_ModDBAProfile_NewFix() const { return m_constants["ModDBAProfile_NewFix"]; }
	QString constant_ModDBAProfile_NewAssure() const { return m_constants["ModDBAProfile_NewAssure"]; }
	QString constant_ModDBAProfile_NewMax() const { return m_constants["ModDBAProfile_NewMax"]; }
	QString constant_ModDBAProfile_NewCompensation() const { return m_constants["ModDBAProfile_NewCompensation"]; }

	// Prompts
	const QRegExp &loginPrompt() const { return m_loginPrompt; }
	const QRegExp &cmdPrompt() const { return m_cmdPrompt; }

	// Error strings.
	const ErrorStrings &commandErrors() const { return m_cmdErrors; }
	const ErrorStrings &loginErrors() const { return m_loginErrors; }

	// Commands.
	QString commandEnableAdmin() const { return m_oltCommands.value("EnableAdmin"); }
	QString commandEnterConfigMode() const { return m_oltCommands.value("EnterConfig"); }
	QString quitContext() const { return m_oltCommands.value("QuitContext"); }

	QString setScroll(int lines) const { return parseCommand(m_oltCommands.value("SetScroll"), QStringList() << "{lines}" << QString::number(lines)); }
	QString getBoardInfo(int frame, int slot) const { return parseCommand(m_oltCommands.value("BoardInfo"), QStringList() << "{frame}" << QString::number(frame) << "{slot}" << QString::number(slot)); }
	QString getUnmanaged() const { return m_oltCommands.value("GetUnmanaged"); }
	QString getONTWANInfo(int frame, int slot, int port, int ontid) const
		{ return parseCommand(m_oltCommands.value("GetONTWANInfo"), QStringList()
							  << "{frame}" << QString::number(frame)
							  << "{slot}" << QString::number(slot)
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontid) ); }

	QString getONTMACInfo(int frame, int slot, int port, int ontid) const
		{ return parseCommand(m_oltCommands.value("GetONTMACInfo"), QStringList()
							  << "{frame}" << QString::number(frame)
							  << "{slot}" << QString::number(slot)
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontid) ); }

	QString getONTVersion(int frame, int slot, int port, int ontid) const
		{ return parseCommand(m_oltCommands.value("GetONTVersion"), QStringList()
							  << "{frame}" << QString::number(frame)
							  << "{slot}" << QString::number(slot)
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontid) ); }

	QString getGPONServiceProfiles() const { return m_oltCommands.value("GetGPONServiceProfiles"); }
	QString getGPONServiceProfile(int serviceProfileID) const
		{ return parseCommand(m_oltCommands.value("GetGPONServiceProfile"), QStringList()
							  << "{srv_profile_id}" << QString::number(serviceProfileID)); }
	QString enterGPONSrvProfile(int serviceProfileID) const
		{ return parseCommand(m_oltCommands.value("EnterGPONSrvPrfl"), QStringList()
							  << "{srv_profile_id}" << QString::number(serviceProfileID)); }

	QString getTrafficTableIPs() const { return m_oltCommands.value("GetTrafficTableIPs");	}
	QString getTrafficTableIP(int index) const
		{ return parseCommand(m_oltCommands.value("GetTrafficTableIP"), QStringList()
							  << "{index}" << QString::number(index) ); }
	QString addTrafficTableIP(const QString &name, int cir, int pir, int priority)
		{ return parseCommand(m_oltCommands.value("AddTrafficTableIP"), QStringList()
							  << "{name}" << name
							  << "{cir}" << QString::number(cir)
							  << "{pir}" << QString::number(pir)
							  << "{priority}" << QString::number(priority) ); }
	QString modTrafficTableIP(int index, const QString &name, int cir, int pir, int priority)
		{ return parseCommand(m_oltCommands.value("ModTrafficTableIP"), QStringList()
							  << "{index}" << QString::number(index)
							  << "{name}" << name
							  << "{cir}" << QString::number(cir)
							  << "{pir}" << QString::number(pir)
							  << "{priority}" << QString::number(priority) ); }
	QString delTrafficTableIP(int index) const
		{ return parseCommand(m_oltCommands.value("DelTrafficTableIP"), QStringList()
							  << "{index}" << QString::number(index) ); }

	QString getDBAProfiles() const { return m_oltCommands.value("GetDBAProfiles"); }
	QString getDBAProfile(int id) const
		{ return parseCommand(m_oltCommands.value("GetDBAProfile"), QStringList()
							  << "{id}" << QString::number(id) ); }

private:
	static QString createDBASpeedsCommandText(int type, int fix, int assured, int max, bool compensate)
	{
		/*
		 * The actual speed text is up to type:
		 * type1                 Fixed bandwidth
		 * type2                 Assured bandwidth
		 * type3                 Assured bandwidth, Maximum bandwidth
		 * type4                 Maximum bandwidth
		 * type5                 Fixed bandwidth, Assured bandwidth, Maximum bandwidth
		 * Furthermore, the type1 needs bandwidth_compensate additional argument:
		 * dba-profile add profile-name asas type1 fix 1234 bandwidth_compensate yes
		 */
		switch( type )
		{
		case 1: return QString("fix %1 bandwidth_compensate %2").arg(fix).arg(compensate ? "yes" : "no");
		case 2: return QString("assure %1").arg(assured);
		case 3:	return QString("assure %1 max %2").arg(assured).arg(max);
		case 4: return QString("max %1").arg(max);
		case 5:	return QString("fix %1 assure %2 max %3").arg(fix).arg(assured).arg(max);
		}
		return "\n";
	}

public:
	QString addDBAProfile(const QString &name, int type, int fix, int assured, int max, bool compensate) const
	{
		return parseCommand(m_oltCommands.value("AddDBAProfile"), QStringList()
							<< "{name}" << name
							<< "{type}" << QString::number(type)
							<< "{speed_text}" << createDBASpeedsCommandText(type, fix, assured, max, compensate));
	}
	QString modDBAProfile(int id, const QString &name) const
	{
		Q_UNUSED(name);
		return parseCommand(m_oltCommands.value("ModDBAProfile"), QStringList()
							<< "{id}" << QString::number(id) );
	}
	QString delDBAProfile(int id) const
		{ return parseCommand(m_oltCommands.value("DelDBAProfile"), QStringList()
							  << "{id}" << QString::number(id) ); }

	QString setTR69Server(int port, int ontID, int tr69ID) const
		{ return parseCommand(m_oltCommands.value("SetTR69Server"), QStringList()
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontID)
							  << "{tr069id}" << QString::number(tr69ID) ); }
	QString unsetTR69Server(int port, int ontID) const
		{ return parseCommand(m_oltCommands.value("UnsetTR69Server"), QStringList()
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontID) ); }

	QString setONTIPConfig(int port, int ontID, int vlan, int priority) const
		{ return parseCommand(m_oltCommands.value("SetONTIPConfig"), QStringList()
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontID)
							  << "{vlan}" << QString::number(vlan)
							  << "{priority}" << QString::number(priority) ); }
	QString unsetONTIPConfig(int port, int ontID) const
		{ return parseCommand(m_oltCommands.value("UnsetONTIPConfig"), QStringList()
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontID) ); }

	QString manageONT(int port, const QString &serial, int lineprofile, int srvprofile, const QString &description)
		{ return parseCommand(m_oltCommands.value("ManageONT"), QStringList()
							  << "{port}" << QString::number(port)
							  << "{serial}" << serial
							  << "{lineprofile}" << QString::number(lineprofile)
							  << "{srvprofile}" << QString::number(srvprofile)
							  << "{desc}" << description ); }

	QString unmanageONT(int port, int ontID) const
		{ return parseCommand(m_oltCommands.value("UnmanageONT"), QStringList()
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontID) ); }

	QString enterGPONContext(int frame, int slot) const
		{ return parseCommand(m_oltCommands.value("EnterGPONContext"), QStringList()
							  << "{frame}" << QString::number(frame)
							  << "{slot}" << QString::number(slot) ); }
	QString exitGPONContext() const { return m_oltCommands.value("QuitGPONContext"); }

	QString getServicePorts() const
		{ return m_oltCommands.value("GetServicePorts"); }
	QString getServicePort(int index)
		{ return parseCommand(m_oltCommands.value("GetServicePort"), QStringList()
							  << "{index}" << QString::number(index) ); }
	QString getONTServicePort(int frame, int slot, int port, int ontid)
		{ return parseCommand(m_oltCommands.value("GetONTServicePort"), QStringList()
							  << "{frame}" << QString::number(frame)
							  << "{slot}" << QString::number(slot)
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontid) ); }
	QString addServicePortLimited(int oltVLAN, int frame, int slot, int port, int ontid, int gem, int ontVLAN, int upTraffic, int downTraffic)
		{ return parseCommand(m_oltCommands.value("AddServicePortLimited"), QStringList()
							  << "{olt_vlan}" << QString::number(oltVLAN)
							  << "{ont_vlan}" << QString::number(ontVLAN)
							  << "{frame}" << QString::number(frame)
							  << "{slot}" << QString::number(slot)
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontid)
							  << "{gem}" << QString::number(gem)
							  << "{dw_traffic_index}" << QString::number(downTraffic)
							  << "{up_traffic_index}" << QString::number(upTraffic) ); }
	QString addServicePortUnlimited(int oltVLAN, int frame, int slot, int port, int ontid, int gem, int ontVLAN)
		{ return parseCommand(m_oltCommands.value("AddServicePortUnlimited"), QStringList()
							  << "{olt_vlan}" << QString::number(oltVLAN)
							  << "{ont_vlan}" << QString::number(ontVLAN)
							  << "{frame}" << QString::number(frame)
							  << "{slot}" << QString::number(slot)
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontid)
							  << "{gem}" << QString::number(gem) ); }
	QString delServicePort(int index) const
		{ return parseCommand(m_oltCommands.value("DelServicePort"), QStringList()
							  << "{index}" << QString::number(index) ); }

	QString resetONT(int port, int ontID) const
		{ return parseCommand(m_oltCommands.value("ResetONT"), QStringList()
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontID) ); }
	QString rebootONT(int port, int ontID, int gracetime) const
		{ return parseCommand(m_oltCommands.value("RebootONT"), QStringList()
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontID)
							  << "{gracetime}" << QString::number(gracetime) ); }

	QString getGPONLineProfiles() const
		{ return parseCommand(m_oltCommands.value("GetLineProfiles"), QStringList()
							  << "{gepon}" << "gpon" ); }
	QString getGPONLineProfile(int id) const
		{ return parseCommand(m_oltCommands.value("GetLineProfile"), QStringList()
							  << "{gepon}" << "gpon"
							  << "{id}" << QString::number(id) ); }
	QString addGPONLineProfile(const QString &name) const
		{ return parseCommand(m_oltCommands.value("AddLineProfile"), QStringList()
							  << "{gepon}" << "gpon"
							  << "{name}" << name ); }
	QString delGPONLineProfile(int id) const
		{ return parseCommand(m_oltCommands.value("DelLineProfile"), QStringList()
							  << "{gepon}" << "gpon"
							  << "{id}" << QString::number(id) ); }
	QString modGPONLineProfile(int id, const QString &name) const
		{ return parseCommand(m_oltCommands.value("GetLineProfile"), QStringList()
							  << "{gepon}" << "gpon"
							  << "{profile}" << QString::number(id)
							  << "{name}" << name ); }
};

extern OLTConstants oltConstants;

#endif // QOLTCONSTANTS_H
