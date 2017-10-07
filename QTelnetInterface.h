#ifndef QTELNETINTERFACE_H
#define QTELNETINTERFACE_H

#include <QStringList>
#include <QMap>
#include <QQueue>

#include "../QTelnet/QTelnet.h"
#include "OLTConstants.h"

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
		OLTConstants::ErrorStrings errorStrings;
		QString prompt;
		OLTState state;	// State on succefull command (on prompt match)
		void clear()
		{
			label.clear();
			cmd.clear();
			prompt.clear();
			state = OltUnknownSate;
		}
		bool isCleared()
		{
			return prompt.isEmpty();
		}
	};

	OLTState m_OLTState;
	QString m_dataBuffer;
	QString m_uname;
	QString m_upass;
	CommandControl m_currentCommand;
	QQueue<CommandControl> m_commandsQueue;

	void setOltState(OLTState newState);
	void playQueue();

protected:
	void addCommand( const QString &label, const QString &cmd, const QString &prompt, const OLTConstants::ErrorStrings &errors, OLTState okState = OltUnknownSate );
	void addLoginCommand( const QString &label, const QString &cmd, const QString &prompt, OLTState okState = OltUnknownSate )
	{ addCommand(label, cmd, prompt, oltConstants.loginErrors(), okState ); }
	void addCommand( const QString &label, const QString &cmd, const QString &prompt, OLTState okState = OltUnknownSate )
	{ addCommand(label, cmd, prompt, oltConstants.commandErrors(), okState ); }

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
