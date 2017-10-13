#ifndef OLTCOMMANDS_SERVICEPORT_H
#define OLTCOMMANDS_SERVICEPORT_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands
{
struct ServicePortFlow
{
	OLTStringValue type;
	OLTStringValue parameter;
public:
	void clear()
	{
		type.clear();
		parameter.clear();
	}
	QString toString()const { return QObject::tr("Type='%1' Parameter='%2'").arg(type.toString(), parameter.toString()); }
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
class ServicePortAdminStatus
{
	enum
	{
		Undefined,
		Unknown,
		Enabled,
		Disabled
	}m_state;
	QString m_data;
public:
	ServicePortAdminStatus() : m_state(Undefined)
	{	}
	ServicePortAdminStatus(const QString &s)
	{ fromString(s); }
	ServicePortAdminStatus(const ServicePortAdminStatus &s) : m_state(s.m_state), m_data(s.m_data)
	{	}
	void fromString(const QString &s)
	{
		if( s == "enable" )		{ m_data = "enable";	m_state = Enabled; }
		else
		if( s == "disable" )	{ m_data = "disable";	m_state = Enabled; }
		else
		if( s == "-" )			{ m_data = "-";			m_state = Undefined;}
		else
		{
			m_data = s;
			m_state = Unknown;
		}
	}
	QString toString() const
	{
		return m_data;
	}
};

struct ServicePortInfoBase : public OntBasicInfo
{
	OLTIntValue index;
	OLTIntValue vlan;
	OLTStringValue vlanAttrib;
	PortType portType;
	OLTIntValue gemportIndex;
	ServicePortFlow flow;
	OLTIntValue rxTrafficTableIndex;
	OLTIntValue txTrafficTableIndex;
	ServicePortState state;
	void clear()
	{
		index.clear();
		vlan.clear();
		vlanAttrib.clear();
		gemportIndex.clear();
		flow.clear();
		rxTrafficTableIndex.clear();
		txTrafficTableIndex.clear();
		state.clear();
	}
	virtual QStringList toStringInfoData() const
	{
		return OntBasicInfo::toStringInfoData()
				<< QObject::tr("Index") << index.toString()
				<< QObject::tr("VLAN ID") << vlan.toString()
				<< QObject::tr("VLAN ATTRIB") << vlanAttrib.toString()
				<< QObject::tr("Port type") << portType.toString()
				<< QObject::tr("VCI (GEM index for GPON)") << gemportIndex.toString()
				<< QObject::tr("Flow") << flow.toString()
				<< QObject::tr("Rx traffic table IP index") << rxTrafficTableIndex.toString()
				<< QObject::tr("Tx traffic table IP index") << txTrafficTableIndex.toString()
				<< QObject::tr("State") << state.toString();
	}
	virtual ~ServicePortInfoBase()
	{	}
};

struct ServicePortInfo : ServicePortInfoBase
{
	OLTStringValue txTrafficTableName;
	OLTStringValue rxTrafficTableName;
	ServicePortAdminStatus adminStatus;
	OLTStringValue label;
	Priority priority;
	OLTStringValue pvcBundle; // Grupo "private virtual circuit"
	OLTIntValue maxMACCount;
	OLTStringValue tagTransform;
	OLTStringValue descrip;
	OLTStringValue remoteDescrip;
	OLTStringValue srvPortBundle;
	OLTStringValue cos;
	OLTIntValue carGroup;
	OLTStringValue staticMAC;
	OLTStringValue ipAddress;
	OLTStringValue statPerformance;
	virtual QStringList toStringInfoData() const
	{
		return ServicePortInfoBase::toStringInfoData()
				<< QObject::tr("Tx Traffic Table Name") << txTrafficTableName.toString()
				<< QObject::tr("Rx Traffic Table Name") << rxTrafficTableName.toString()
				<< QObject::tr("Admin status") << adminStatus.toString()
				<< QObject::tr("Etiqueta") << label.toString()
				<< QObject::tr("Prioridad") << priority.toString()
				<< QObject::tr("Grupo PVC") << pvcBundle.toString()
				<< QObject::tr("Máxima cantidad MACs") << maxMACCount.toString()
				<< QObject::tr("Tag transform") << tagTransform.toString()
				<< QObject::tr("Descripción") << descrip.toString()
				<< QObject::tr("Descripción remota") << remoteDescrip.toString()
				<< QObject::tr("Grupo service-port") << srvPortBundle.toString()
				<< QObject::tr("Cos") << cos.toString()
				<< QObject::tr("car-group") << carGroup.toString()
				<< QObject::tr("MAC estática") << staticMAC.toString()
				<< QObject::tr("Dirección IP") << ipAddress .toString()
				<< QObject::tr("stat performance") << statPerformance.toString();
	}
	virtual ~ServicePortInfo()
	{	}
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
	ServicePortInfo m_srvPort;

public:
	ServicePort(const QString &label,const QString &command, const QString &rawData);
	virtual QStringList toStringInfoData() const
	{
		return m_srvPort.toStringInfoData();
	}
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
