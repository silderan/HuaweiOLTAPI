#include "QOLTCommands.h"

QOLTCommands::QOLTCommands()
{
	connect(this, SIGNAL(newResponse(QString,QString,QString)), this, SLOT(onCommandReceived(QString,QString,QString)));
}

void QOLTCommands::setAdminMode()
{
	if( oltState() < QTelnetInterface::OltAdminMode )
		addCommand( "EnablingAdmin", oltConstants.commandEnableAdmin(), oltConstants.promptAdmin(), OltAdminMode );
}

void QOLTCommands::setConfigMode()
{
	if( oltState() < QTelnetInterface::OltConfigMode )
	{
		setAdminMode();
		addCommand( "ConfigMode", oltConstants.commandEnterConfigMode(), oltConstants.promptConfig(), OltConfigMode );
	}
}

void QOLTCommands::setScroll(int lines)
{
	addCommand("Scroll", oltConstants.scroll(lines), oltConstants.promptConfig());
}

void QOLTCommands::getBoardInfo(quint8 frame, quint8 slot)
{
	addCommand("BoardInfo", oltConstants.boardInfo(frame, slot), oltConstants.promptConfig());
}

void QOLTCommands::getUnmanaged()
{
	addCommand("GetUnmanaged", oltConstants.unmanaged(), oltConstants.promptConfig());
}

void QOLTCommands::onCommandReceived(const QString &tag, const QString &cmd, const QString data)
{
	if( tag == "BoardInfo" )
		emit boardInfo( BoardInfo(tag, cmd, data) );
	else if( tag == "GetUnmanaged" )
		emit unmanagedOnts( UnmanagedONTs(tag, cmd, data) );
}
