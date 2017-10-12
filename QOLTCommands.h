#ifndef OLTCOMMANDS_H
#define OLTCOMMANDS_H

#include "QTelnetInterface.h"
#include "OLTCommands_BoardInfo.h"
#include "OLTCommands_UnmanagedOLTs.h"
#include "OLTCommands_ONTWANInfo.h"
#include "OLTCommands_ONTMACInfo.h"
#include "OLTCommands_ONTVersion.h"
#include "OLTCommands_GPONServiceProfile.h"
#include "OLTCommands_TrafficTableIP.h"
#include "OLTCommands_DBAProfile.h"
#include "OLTCommands_LineProfile.h"
#include "OLTCommands_ServicePort.h"

namespace OLTCommands
{
class QOLTCommands : public QTelnetInterface
{
Q_OBJECT

public:
	QOLTCommands();
	void setAdminMode();
	void setConfigMode();
	void quitContext();
	void setScroll(int lines);
	void getBoardInfo(quint8 frame, quint8 slot);
	void getUnmanaged();
	void getONTWANInfo(int frame, int slot, int port, int ontid);
	void getONTMACInfo(int frame, int slot, int port, int ontid);
	void getONTVersion(int frame, int slot, int port, int ontid);

	void getGPONServiceProfiles();
	void getGPONServiceProfile(int serviceProfileID);
	void enterGPONSrvcPrfl(int serviceProfileID);

	void getTrafficTableIPs();
	void getTrafficTableIP(int index);
	void addTrafficTableIP(const QString &name, int cir, int pir, int priority);
	void modTrafficTableIP(int index, const QString &name, int cir, int pir, int priority);
	void delTrafficTableIP(int index);

	void getDBAPProfiles();
	void getDBAPProfile(int index);
	void addDBAPProfile(const QString &name, const QString &type, const QString &speeds);
//	void modDBAPProfile(int index, const QString &name, const QString &type, const QString &speeds);
	void delDBAPProfile(int index);

	void getServicePorts(int frame, int slot, int port, int ontid);
	void getServicePort(int frame, int slot, int port, int ontid, int index);
	void addServicePort(int frame, int slot, int port, int ontid, int oltVLAN, int ontVLAN, int gemport, int upTrafficIndex = -1, int dwTrafficIndex = -1);
//	void modServicePort(int frame, int slot, int port, int ontid, int oltVLAN, int ontVLAN, int gemport, int upTrafficIndex = -1, int dwTrafficIndex = -1);
	void delServicePort(int index);

	void getLineProfiles(int gepon);
	void getLineProfile(int gepon, int index, const QString &name);
	void addLineProfile(int gepon, const QString &name);
//	void modLineProfile(int index, const QString &name, const QString &type, const QString &speeds);
	void delLineProfile(int gepon, int index);

private slots:
	void onCommandReceived(const QString &tag, const QString &cmd, const QString data);

signals:
	void boardInfo(const OLTCommands::BoardInfo &);
	void unmanagedOnts(const OLTCommands::UnmanagedONTs &);
	void ontWANInfo(const OLTCommands::ONTWANInfo &);
	void ontMACInfo(const OLTCommands::ONTMACInfo &);
	void ontVersionInfo(const OLTCommands::ONTVersion &);

	void gponServiceProfiles(const OLTCommands::GPONServiceProfiles &);
	void gponServiceProfile(const OLTCommands::GPONServiceProfile &);

	void trafficTableIPs(const OLTCommands::TrafficTableIPs &);
	void trafficTableIP(const OLTCommands::TrafficTableIP &);

	void dbaProfiles(const OLTCommands::DBAProfiles &);
	void dbaProfile(const OLTCommands::DBAProfile &);

	void servicePorts(const OLTCommands::ServicePorts &);
	void servicePort(const OLTCommands::ServicePort &);

	void lineProfiles(const OLTCommands::LineProfiles &);
	void lineProfile(const OLTCommands::LineProfile &);
};
}// End namespace.

#endif // OLTCOMMANDS_H
