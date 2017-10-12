#ifndef OLTCOMMANDS_SERVICEPORT_H
#define OLTCOMMANDS_SERVICEPORT_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands
{
struct ServicePortFlow
{
	QString type;
	QString parameter;
public:
	void clear()
	{
		type.clear();
		parameter.clear();
	}
	QString toString()const { return QObject::tr("Type='%1' Parameter='%2'").arg(type, parameter); }
};
class ServicePortState
{
	enum
	{
		Unkown = -2,
		Undefined = -1,
		Up,
		Down
	}m_state;
public:
	void fromString(const QString &s)
	{
		if( s == "up" )
			m_state = Up;
		else
		if( s == "down" )
			m_state = Down;
		else
			m_state = Unkown;
	}
	QString toString() const
	{
		switch(m_state)
		{
		case Unkown:	return "unknown";
		case Undefined:	return "undefined";
		case Up:		return "up";
		case Down:		return "down";
		}
		return "unknown";
	}
	void clear() { m_state = Unkown;	}
	bool isCleared() const	{ return m_state == Unkown;	}
	bool isUp() const		{ return m_state == Up;		}
	bool isDown() const		{ return m_state == Down;	}

	ServicePortState() : m_state(Unkown)
	{	}
	ServicePortState(const ServicePortState &s) : m_state(s.m_state)
	{	}
	ServicePortState(const QString &s)
	{	fromString(s);	}
};

struct ServicePortInfoBase : public OntBasicInfo
{
	int index;
	int vlan;
	QString vlanAttrib;
	PortType portType;
	int vpi;
	int vci;
	ServicePortFlow flow;
	int rxTrafficTableIndex;
	int txTrafficTableIndex;
	ServicePortState state;
	void clear()
	{
		index = -1;
		vlan = -1;
		vlanAttrib.clear();
		vpi = -1;
		vci = -1;
		flow.clear();
		rxTrafficTableIndex = -1;
		txTrafficTableIndex = -1;
		state.clear();
	}
	virtual QStringList toStringInfoData() const
	{
		return QStringList()
				<< QObject::tr("Index") << QString::number(index)
				<< QObject::tr("VLAN ID") << QString::number(vlan)
				<< QObject::tr("VLAN ATTRIB") << vlanAttrib
				<< QObject::tr("Port type") << portType.toString()
				<< QObject::tr("VPI (ONT ID for PON)") << QString::number(vpi)
				<< QObject::tr("VCI (GEM index for GPON)") << QString::number(vci)
				<< QObject::tr("Flow") << flow.toString()
				<< QObject::tr("Rx traffic table IP index") << QString::number(rxTrafficTableIndex)
				<< QObject::tr("Tx traffic table IP index") << QString::number(txTrafficTableIndex)
				<< QObject::tr("State") << state.toString();
	}
	virtual ~ServicePortInfoBase()
	{	}
};

struct ServicePortInfo : ServicePortInfoBase
{

};

class ServicePortInfoList : public QList<ServicePortInfoBase>
{
public:
	virtual QStringList toStringInfoData() const
	{
		QStringList rtn;
		for( int i = 0; i < count(); i++ )
		{
			rtn << "" << QObject::tr("Service port n.%1").arg(i) << at(i).toStringInfoData();
		}
		return rtn;
	}
};

class ServicePort : public OLTCommandReply
{
	ServicePortInfo m_servicePort;

public:
	ServicePort(const QString &label,const QString &command, const QString &rawData);
};

class ServicePorts : public OLTCommandReply
{
	ServicePortInfoList m_srvPorts;

public:
	ServicePorts(const QString &label,const QString &command, const QString &rawData);
	virtual QStringList toStringInfoData() const
	{
		return m_srvPorts.toStringInfoData();
	}
};
} // End namespace
#endif // OLTCOMMANDS_SERVICEPORT_H
