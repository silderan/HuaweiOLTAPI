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
	QOLTCommands huaweiOLT;

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
	void on_btNewOLTs_clicked();
	void on_btScroll_clicked();

	void on_btBoardInfo_clicked();
	void boardInfoReceived(const BoardInfo &boardInfo);
	void unmanagedReceived(const UnmanagedONTs &unmanaged);
};

#endif // QFRONTEND_H
