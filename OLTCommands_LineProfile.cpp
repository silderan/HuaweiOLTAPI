#include "OLTCommands_LineProfile.h"

namespace OLTCommands
{
OLTCommands::LineProfile::LineProfile(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{

}

OLTCommands::LineProfiles::LineProfiles(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{

}
}
