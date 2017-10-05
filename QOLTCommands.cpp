#include "QOLTCommands.h"


OLTCommands::OLTCommands()
{
}

void OLTCommands::enableAdminMode()
{
	if( oltState() < QTelnetInterface::OltAdminMode )
		addCommand( "EnablingAdmin", oltConstants.commandEnableAdmin(), oltConstants.promptAdmin(), OltAdminMode );
}

void OLTCommands::enterConfigMode()
{
	if( oltState() < QTelnetInterface::OltConfigMode )
	{
		enableAdminMode();
		addCommand( "ConfigMode", oltConstants.commandEnterConfigMode(), oltConstants.promptConfig(), OltConfigMode );
	}
}

void OLTCommands::scroll(int lines)
{
	addCommand("Scroll", oltConstants.scroll(lines), oltConstants.promptConfig());
}
