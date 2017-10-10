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
	public:
		QString moreText() const;
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
	QString constantMoreText() const { return m_constants.moreText();	}

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

	QString scroll(int lines) const { return parseCommand(m_oltCommands.value("scroll"), QStringList() << "{lines}" << QString::number(lines)); }
	QString boardInfo(int frame, int slot) const { return parseCommand(m_oltCommands.value("BoardInfo"), QStringList() << "{frame}" << QString::number(frame) << "{slot}" << QString::number(slot)); }
	QString unmanaged() const { return m_oltCommands.value("GetUnmanaged"); }
	QString ontWANInfo(int frame, int slot, int port, int ontid) const
		{ return parseCommand(m_oltCommands.value("ONTWANInfo"), QStringList()
							  << "{frame}" << QString::number(frame)
							  << "{slot}" << QString::number(slot)
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontid) ); }

	QString ontMACInfo(int frame, int slot, int port, int ontid) const
		{ return parseCommand(m_oltCommands.value("ONTMACInfo"), QStringList()
							  << "{frame}" << QString::number(frame)
							  << "{slot}" << QString::number(slot)
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontid) ); }

	QString ontVersion(int frame, int slot, int port, int ontid) const
		{ return parseCommand(m_oltCommands.value("ONTVersion"), QStringList()
							  << "{frame}" << QString::number(frame)
							  << "{slot}" << QString::number(slot)
							  << "{port}" << QString::number(port)
							  << "{ontid}" << QString::number(ontid) ); }

	QString gponServiceProfiles() const { return m_oltCommands.value("GPONServiceProfiles"); }
	QString gponServiceProfile(int serviceProfileID) const
		{ return parseCommand(m_oltCommands.value("GPONServiceProfile"), QStringList()
							  << "{srv_profile_id}" << QString::number(serviceProfileID)); }
	QString enterGPONSrvProfile(int serviceProfileID) const
		{ return parseCommand(m_oltCommands.value("EnterGPONSrvPrfl"), QStringList()
							  << "{srv_profile_id}" << QString::number(serviceProfileID)); }

	QString trafficTableIP(int index) const
		{ return parseCommand(m_oltCommands.value("TrafficTableIPInfo"), QStringList()
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
	QString getDBAProfile(int index) const
		{ return parseCommand(m_oltCommands.value("GetDBAProfile"), QStringList()
							  << "{index}" << QString::number(index) ); }
	QString addDBAProfile(const QString &name, const QString &type, const QString &speeds) const
		{ return parseCommand(m_oltCommands.value("AddDBAProfile"), QStringList()
							  << "{name}" << name
							  << "{type}" << type
							  << "{speeds}" << speeds ); }
	QString delDBAProfile(int index) const
		{ return parseCommand(m_oltCommands.value("DelDBAProfile"), QStringList()
							  << "{index}" << QString::number(index) ); }

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

	QString ontServicePorts(int frame, int slot, int port, int ontid) const
		{ return parseCommand(m_oltCommands.value("ONTServicePorts"), QStringList()
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
	QString getLineProfiles(int port) const
		{ return parseCommand(m_oltCommands.value("GetLineProfiles"), QStringList()
							  << "{port}" << QString::number(port) ); }
	QString getLineProfile(int port, int profileID, const QString &profileName) const
		{ return parseCommand(m_oltCommands.value("GetLineProfile"), QStringList()
							  << "{port}" << QString::number(port)
							  << "{profile}" << QString::number(profileID)
							  << "{name}" << profileName ); }
	QString addLineProfile(int port, const QString &profileName) const
		{ return parseCommand(m_oltCommands.value("AddLineProfile"), QStringList()
							  << "{port}" << QString::number(port)
							  << "{name}" << profileName ); }
	QString delLineProfile(int port, int profileID) const
		{ return parseCommand(m_oltCommands.value("DelLineProfile"), QStringList()
							  << "{port}" << QString::number(port)
							  << "{profile}" << QString::number(profileID) ); }
	QString modLineProfile(int port, int profileID, const QString &profileName) const
		{ return parseCommand(m_oltCommands.value("GetLineProfile"), QStringList()
							  << "{port}" << QString::number(port)
							  << "{profile}" << QString::number(profileID)
							  << "{name}" << profileName ); }
};

extern OLTConstants oltConstants;

#endif // QOLTCONSTANTS_H
