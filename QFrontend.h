#ifndef QFRONTEND_H
#define QFRONTEND_H

#include <QMainWindow>

#include "QOLTCommands.h"

namespace Ui
{
	class QFrontend;
}

class QFrontend : public QMainWindow
{
Q_OBJECT
	Ui::QFrontend *ui;
	OLTCommands::QOLTCommands huaweiOLT;

	enum CommandIndex
	{
		CmdScroll,
		CmdGetUnmanaged,
		CmdBoardInfo,
		CmdONTInfo,
		CmdONTWANInfo,
		CmdONTMACInfo,
		CmdONTVersion,
		CmdGPONServiceProfiles,
		CmdGPONServiceProfile,
		CmdEnterSrvPrfl,
		CmdTrafficTableIPs,
		CmdTrafficTableIP,
		CmdAddTrafficTableIP,
		CmdModTrafficTableIP,
		CmdDelTrafficTableIP
	};
	QList<QWidget*> m_paramWidgets;

public:
	explicit QFrontend(QWidget *parent = 0);
	~QFrontend();

protected:
	void addViewerText(const QString &text);

private slots:
	void oltTelnetIncommings(const char *data, int length);
	void oltTelnetStatusChanged(QAbstractSocket::SocketState state);
	void oltStatusChanged(QTelnetInterface::OLTState state);
	void oltErrorResponse(const QString &tag, const QString &cmd, const QString &err);

	void on_btLogin_clicked();

	void boardInfoReceived(const OLTCommands::BoardInfo &boardInfo);
	void unmanagedReceived(const OLTCommands::UnmanagedONTs &unmanaged);
	void ontsWANInfoReceived(const OLTCommands::ONTWANInfo &ontWANInfo);
	void ontsMACInfoReceived(const OLTCommands::ONTMACInfo &ontMACInfo);
	void ontVersionReceived(const OLTCommands::ONTVersion &ontVersion);

	void gponSrvPrfsReceived(const OLTCommands::GPONServiceProfiles &gponSrvProfiles);
	void gponSrvPrfReceived(const OLTCommands::GPONServiceProfile &gponSrvProfile);

	void trafficTableIPsReceived(const OLTCommands::TrafficTableIPs &trafficTableIPs);
	void trafficTableIPReceived(const OLTCommands::TrafficTableIP &trafficTableIP);

	void on_command_currentIndexChanged(int index);
	void on_sendCMD_clicked();
	void on_quit_clicked();
	void on_btCmd_clicked();
};

#endif // QFRONTEND_H
