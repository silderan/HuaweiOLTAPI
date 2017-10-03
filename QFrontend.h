#ifndef QFRONTEND_H
#define QFRONTEND_H

#include <QMainWindow>

#include "QTelnetInterface.h"

namespace Ui
{
	class QFrontend;
}

class QFrontend : public QMainWindow
{
Q_OBJECT
	Ui::QFrontend *ui;
	QTelnetInterface huaweiOLT;

public:
	explicit QFrontend(QWidget *parent = 0);
	~QFrontend();

protected:
	void addViewerText(const QString &text);
private slots:
	void oltTelnetIncommings(const char *data, int length);
	void oltTelnetStatusChanged(QAbstractSocket::SocketState state);
	void on_btLogin_clicked();
	void on_btNewOLTs_clicked();
};

#endif // QFRONTEND_H
