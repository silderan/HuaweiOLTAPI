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

protected:
	class ErrorStrings : public QMap<QString, QString>
	{
	public:
		void addErrorString(const QString &match, const QString &explain);
		QString errorString(const QString &text) const;
	};

public:
	enum OLTState
	{
		OltUnknownSate = -1,
		OltUnconnected,
		OltConnected,
		OltLogging,
		OltLogged,
		OltAdminMode,
		OltConfigMode
	};

private:
	struct CommandControl
	{
		QString label;
		QString cmd;
		ErrorStrings errorStrings;
		QString prompt;
		OLTState state;	// State on succefull command (on prompt match)
	};

	ErrorStrings m_loginErrors;
	OLTState m_OLTState;
	QString m_dataBuffer;
	CommandControl m_currentCommand;
	QQueue<CommandControl> m_commands;

	void setOltState(OLTState newState);
	void playQueue();

protected:
	void addCommand( const QString &label, const QString &cmd, const QString &promtp, const ErrorStrings &errors, OLTState okState = OltUnknownSate );

public:
	QTelnetInterface();
	OLTState oltState() const { return m_OLTState; }
	void connectToOLT(const QString &host, quint16 port, const QString &uname, const QString &upass);

private slots:
	void onDataFromOLT(const char *data, int length);
	void onTelnetStateChange(QAbstractSocket::SocketState st);

signals:
	void oltStateChanged(QTelnetInterface::OLTState s);
	void newResponse(const QString &label, const QString &cmd, const QString &response);
	void errorResponse(const QString &label, const QString &cmd, const QString &error);
};

#endif // QTELNETINTERFACE_H
