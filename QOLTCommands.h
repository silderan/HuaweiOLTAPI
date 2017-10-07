#ifndef OLTCOMMANDS_H
#define OLTCOMMANDS_H

#include "QTelnetInterface.h"
#include "OLTCommands_BoardInfo.h"
#include "OLTCommands_UnmanagedOLTs.h"

namespace OLTCommands { class QOLTCommands; }

class QOLTCommands : public QTelnetInterface
{
Q_OBJECT

public:
	QOLTCommands();
	void setAdminMode();
	void setConfigMode();
	void setScroll(int lines);
	void getBoardInfo(quint8 frame, quint8 slot);
	void getUnmanaged();

private slots:
	void onCommandReceived(const QString &tag, const QString &cmd, const QString data);

signals:
	void boardInfo(const BoardInfo &getBoardInfo);
	void unmanagedOnts(const UnmanagedONTs &unmanaged);
};

#endif // OLTCOMMANDS_H
