#include "QTelnetInterface.h"


/**
 * @brief QTelnetInterface::addCommand
 * Adds a new command to the queue.
 * @param cmd The command to send.
 * @param promtp The prompt to wait to for a succesfull command.
 * @param errors Lists of paired texts for errors and their explanation text.
 */
void QTelnetInterface::addCommand(const QString &label, const QString &cmd, const QString &promtp, const QStringList &errors)
{
	CommandControl newCmd;
	newCmd.label = label;
	newCmd.cmd = cmd;
	newCmd.promtp = promtp;
	for( int i = 0; i < errors.count(); i+=2 )
		newCmd.errorStrings.insert(errors[i+0], errors[i+1]);
	m_commands.append( newCmd );
	playQueue();
}

QTelnetInterface::QTelnetInterface()
{
	connect( this, SIGNAL(newData(const char*,int)), this, SLOT(onDataFromOLT(const char*,int)) );
	connect( this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onTelnetStateChange(QAbstractSocket::SocketState)) );
}

void QTelnetInterface::connectToOLT(const QString &host, quint16 port, const QString &uname, const QString &upass)
{
	QStringList loginErrors = QStringList()
			<< "Reenter times have reached the upper limit." << tr("User %1 is already loged and cannot log again more times")
			<< "Username or password invalid." << tr("Invalid username or password")
			<< "Username or Domain invalid!" << tr("Invalid username");
	addCommand("InitialWelcome", "", "User name:", loginErrors);
	addCommand("UsernameLogin", uname, "User password:", loginErrors );
	addCommand("PasswordLogin", upass, "\nMA5683T", loginErrors);
	QTelnet::connectToHost(host, port);
}

void QTelnetInterface::playQueue()
{
	if( (oltState() == OltConnected) && m_commands.count() )
	{
		m_currentCommand = m_commands.takeFirst();
		m_dataBuffer.clear();
		if( !m_currentCommand.cmd.isEmpty() )
			sendData(m_currentCommand.cmd.toLatin1());
	}
}

void QTelnetInterface::onDataFromOLT(const char *data, int length)
{
	m_dataBuffer.append( QByteArray(data, length) );
	if( m_dataBuffer.contains(m_currentCommand.promtp) )
		emit newResponce(m_currentCommand.label, m_currentCommand.cmd, m_dataBuffer);
	else
	{
		QMap<QString, QString>::const_iterator i = m_currentCommand.errorStrings.constBegin();
		while( i != m_currentCommand.errorStrings.constEnd() )
		{
			if( m_dataBuffer.contains(i.key()) )
				emit errorResponce(m_currentCommand.label, m_currentCommand.cmd, i.value());
		}
	}
	playQueue();
}

void QTelnetInterface::onTelnetStateChange(QAbstractSocket::SocketState st)
{
	switch( st )
	{
	case QAbstractSocket::UnconnectedState:
		m_OLTState = OltUnconnected;
		break;
	case QAbstractSocket::HostLookupState:
		m_OLTState = OltUnconnected;
		break;
	case QAbstractSocket::ConnectingState:
		m_OLTState = OltUnconnected;
		break;
	case QAbstractSocket::ConnectedState:
		m_OLTState = OltConnected;
		playQueue();
		break;
	case QAbstractSocket::BoundState:
		m_OLTState = OltUnconnected;
		break;
	case QAbstractSocket::ListeningState:
		break;
		m_OLTState = OltUnconnected;
	case QAbstractSocket::ClosingState:
		m_OLTState = OltUnconnected;
		break;
	}
}
