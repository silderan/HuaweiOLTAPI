#include "QTelnetInterface.h"


void QTelnetInterface::ErrorStrings::addErrorString(const QString &match, const QString &explain)
{
	insert(match, explain);
}

QString QTelnetInterface::ErrorStrings::errorString(const QString &text) const
{
	QTelnetInterface::ErrorStrings::const_iterator i = constBegin();
	while( i != constEnd() )
	{
		if( text.contains(i.key()) )
			return i.value();
		i++;
	}
	// No error.
	return QString();
}

/**
 * @brief QTelnetInterface::addCommand
 * Adds a new command to the queue.
 * @param cmd The command to send.
 * @param promtp The prompt to wait to for a succesfull command.
 * @param errors Lists of paired texts for errors and their explanation text.
 */
void QTelnetInterface::addCommand(const QString &label, const QString &cmd, const QString &prompt, const ErrorStrings &errors, OLTState okState)
{
	CommandControl newCmd;
	newCmd.label = label;
	newCmd.cmd = cmd;
	newCmd.prompt = prompt;
	newCmd.errorStrings = errors;
	newCmd.state = okState;
	m_commands.append( newCmd );
	playQueue();
}

void QTelnetInterface::setOltState(QTelnetInterface::OLTState newState)
{
	if( m_OLTState != newState )
	{
		m_OLTState = newState;
		emit oltStateChanged( m_OLTState );
	}
}

QTelnetInterface::QTelnetInterface()
{
	m_loginErrors.addErrorString( "Reenter times have reached the upper limit.", tr("User %1 is already loged and cannot log again more times") );
	m_loginErrors.addErrorString( "Username or password invalid.", tr("Invalid username or password") );
	m_loginErrors.addErrorString( "Username or Domain invalid!", tr("Invalid username") );

	connect( this, SIGNAL(newData(const char*,int)), this, SLOT(onDataFromOLT(const char*,int)) );
	connect( this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onTelnetStateChange(QAbstractSocket::SocketState)) );
}

void QTelnetInterface::connectToOLT(const QString &host, quint16 port, const QString &uname, const QString &upass)
{
	addCommand( "InitialWelcome", "", "User name:", m_loginErrors, OltLogging );
	addCommand( "UsernameLogin", uname, "User password:", m_loginErrors, OltLogging );
	addCommand( "PasswordLogin", upass, "\nMA5683T>", m_loginErrors, OltLogged );
	QTelnet::connectToHost(host, port);
}

void QTelnetInterface::playQueue()
{
	if( (state() == QAbstractSocket::ConnectedState) && m_commands.count() )
	{
		m_currentCommand = m_commands.takeFirst();
		m_dataBuffer.clear();
		if( !m_currentCommand.cmd.isEmpty() )
		{
			sendData( m_currentCommand.cmd.toLatin1() );
			sendData( "\n" );
		}
	}
}

void QTelnetInterface::onDataFromOLT(const char *data, int length)
{
	m_dataBuffer.append( QByteArray(data, length) );
	QString err = m_currentCommand.errorStrings.errorString(m_dataBuffer);
	if( !err.isEmpty() )
	{
		emit errorResponse(m_currentCommand.label, m_currentCommand.cmd, err);
		m_dataBuffer.clear();
	}
	else if( m_dataBuffer.contains(m_currentCommand.prompt) )
	{
		if( m_currentCommand.state != OltUnknownSate )
			setOltState(m_currentCommand.state);
		emit newResponse(m_currentCommand.label, m_currentCommand.cmd, m_dataBuffer);
		m_dataBuffer.clear();
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
