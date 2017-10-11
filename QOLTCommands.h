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

namespace OLTCommands { class QOLTCommands; }

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

private slots:
	void onCommandReceived(const QString &tag, const QString &cmd, const QString data);

signals:
	void boardInfo(const BoardInfo &);
	void unmanagedOnts(const UnmanagedONTs &);
	void ontWANInfo(const ONTWANInfo &);
	void ontMACInfo(const ONTMACInfo &);
	void ontVersionInfo(const ONTVersion &);

	void gponServiceProfiles(const GPONServiceProfiles &);
	void gponServiceProfile(const GPONServiceProfile &);

	void trafficTableIPs(const TrafficTableIPs &);
	void trafficTableIP(const TrafficTableIP &);
};

#endif // OLTCOMMANDS_H
