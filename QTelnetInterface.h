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
		OLTState state;	// State on succefull command (on prompt match)
		void clear()
		{
			label.clear();
			cmd.clear();
			state = OltUnknownSate;
		}
		bool isCleared()
		{
			return label.isEmpty();
		}
	};
	struct AutoResponse
	{
		QRegExp m_pattern;
		QString m_cmd;
		AutoResponse(const QString &pattern, const QString &cmd) :
			m_pattern(pattern), m_cmd(cmd)
		{

		}
	};
	class AutoResponseList : public QList<AutoResponse>
	{
	public:
		void addAutoResponce(const QString &pattern, const QString &cmd)
		{
			append( AutoResponse(pattern, cmd) );
		}

		QString takeAutoResponse(const QString &s)
		{
			for( int i = 0; i < count(); i++ )
			{
				QString tmp = at(i).m_pattern.pattern();
				if( s.contains(at(i).m_pattern) )
					return takeAt(i).m_cmd;
			}
			return QString();
		}
	};

	OLTState m_OLTState;
	QString m_dataBuffer;
	QString m_uname;
	QString m_upass;
	CommandControl m_currentCommand;

	QQueue<CommandControl> m_commandsQueue;
	AutoResponseList m_autoResponseList;

	void setOltState(OLTState newState);
	void playQueue();

	void addCommand(const QString &label, const QString &cmd, const OLTConstants::ErrorStrings &errors, OLTState okState = OltUnknownSate );

protected:
	void addAutoResponce(const QString &pattern, const QString &cmd)
		{ m_autoResponseList.addAutoResponce(pattern, cmd);	}

	void addCommand( const QString &label, const QString &cmd, OLTState okState = OltUnknownSate )
		{ addCommand(label, cmd, oltConstants.commandErrors(), okState ); }

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
