#ifndef QTELNETINTERFACE_H
#define QTELNETINTERFACE_H

#include <QStringList>
#include <QMap>
#include <QQueue>

#include "../QTelnet/QTelnet.h"

/**
 * @brief The QTelnetInterface class
 * This class abstracts huawei telnet interface.
 * Takes care of sending and incomming commands.
 */
class QTelnetInterface : public QTelnet
{
Q_OBJECT

public:
	enum OLTState
	{
		OltUnconnected,
		OltConnected,
		OltLogging,
		OltLogged
	};
	struct CommandControl
	{
		QString label;
		QString cmd;
		QMap<QString, QString> errorStrings;
		QString promtp;
	};

	QString m_dataBuffer;
	CommandControl m_currentCommand;
	QQueue<CommandControl> m_commands;

	void addCommand(const QString &label, const QString &cmd, const QString &promtp, const QStringList &errors = QStringList());
private:
	OLTState m_OLTState;

public:
	QTelnetInterface();
	OLTState oltState() const { return m_OLTState; }
	void connectToOLT(const QString &host, quint16 port, const QString &uname, const QString &upass);

private slots:
	void onDataFromOLT(const char *data, int length);
	void onTelnetStateChange(QAbstractSocket::SocketState st);

protected:
	void playQueue();

signals:
	void newResponce(const QString &label, const QString &cmd, const QString &responce);
	void errorResponce(const QString &label, const QString &cmd, const QString &error);
};

#endif // QTELNETINTERFACE_H
