#include "OLTCommands_ServicePort.h"

namespace OLTCommands
{
OLTCommands::ServicePort::ServicePort(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{

}


OLTCommands::ServicePorts::ServicePorts(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{

}
}
