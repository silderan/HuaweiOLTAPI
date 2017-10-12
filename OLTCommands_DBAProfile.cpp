#include "OLTCommands_DBAProfile.h"

namespace OLTCommands
{
OLTCommands::DBAProfile::DBAProfile(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{

}


OLTCommands::DBAProfiles::DBAProfiles(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{

}
}
