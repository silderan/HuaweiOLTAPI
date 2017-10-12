#ifndef OLTCOMMANDS_SERVICEPORT_H
#define OLTCOMMANDS_SERVICEPORT_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands
{
struct ServicePortInfoBase
{

};

struct ServicePortInfo : ServicePortInfoBase
{

};

class ServicePortInfoList : public QList<ServicePortInfoBase>
{

};

class ServicePort : public OLTCommandReply
{
	ServicePortInfo m_servicePort;

public:
	ServicePort(const QString &label,const QString &command, const QString &rawData);
};

class ServicePorts : public OLTCommandReply
{
	ServicePortInfoList m_servicePorts;

public:
	ServicePorts(const QString &label,const QString &command, const QString &rawData);
};
} // End namespace
#endif // OLTCOMMANDS_SERVICEPORT_H
