#include "QOLTCommands.h"


OLTCommands::OLTCommands()
{
}

void OLTCommands::enableAdminMode()
{
	if( oltState() < QTelnetInterface::OltAdminMode )
		addCommand( "EnablingAdmin", "enablse", oltConstants.promptAdmin(), OltLogging );
}

void OLTCommands::enterConfigMode()
{
	if( oltState() < QTelnetInterface::OltConfigMode )
	{
		enableAdminMode();
		addCommand( "ConfigMode", "config", oltConstants.promptConfig(), OltLogged );
	}
}
