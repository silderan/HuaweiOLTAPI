#ifndef OLTCOMMANDS_H
#define OLTCOMMANDS_H

#include "QTelnetInterface.h"
#include "OLTCommands_BoardInfo.h"
#include "OLTCommands_UnmanagedOLTs.h"
#include "OLTCommands_ONTWANInfo.h"
#include "OLTCommands_ONTMACInfo.h"
#include "OLTCommands_GPONServiceProfile.h"

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
	void getGPONServiceProfiles();
	void getGPONServiceProfile(int serviceProfileID);
	void enterGPONSrvcPrfl(int serviceProfileID);

private slots:
	void onCommandReceived(const QString &tag, const QString &cmd, const QString data);

signals:
	void boardInfo(const BoardInfo &);
	void unmanagedOnts(const UnmanagedONTs &);
	void ontWANInfo(const ONTWANInfo &);
	void ontMACInfo(const ONTMACInfo &);
	void gponServiceProfiles(const GPONServiceProfiles &);
	void gponServiceProfile(const GPONServiceProfile &);
};

#endif // OLTCOMMANDS_H
