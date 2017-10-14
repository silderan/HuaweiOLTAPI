#ifndef OLTCOMMANDS_LINEPROFILE_H
#define OLTCOMMANDS_LINEPROFILE_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands
{
struct LineProfileInfoBase
{
	OLTIntValue id;
	OLTStringValue name;
	int bindingTimes;
	void clear()
	{
		id.clear();
		name.clear();
		bindingTimes = 0;
	}
	virtual QStringList toStringInfoData() const
	{
		return QStringList()
				<< QObject::tr("ID") << id.toString()
				<< QObject::tr("Nombre") << name.toString()
				<< QObject::tr("Veces usado") << QString::number(bindingTimes);
	}
	virtual ~LineProfileInfoBase()
	{	}
};

struct LineProfileInfo : LineProfileInfoBase
{
	PortType accessType;
	OLTStringValue fecUpstreamSwitch;// :Disable
	OLTStringValue omccEncryptSwitch;// :Off
	OLTStringValue qosMode;//            :GEM-CAR
	OLTStringValue mappingMode;//        :VLAN
	OLTStringValue tr69Management;//   :Disable
	OLTIntValue tr69IPIndex;//      :0
	virtual QStringList toStringInfoData() const
	{
		return LineProfileInfoBase::toStringInfoData()
				<< QObject::tr("FEC upstream switch") << fecUpstreamSwitch.toString()
				<< QObject::tr("OMCC encrypt switch") << omccEncryptSwitch.toString()
				<< QObject::tr("Qos mode") << qosMode.toString()
				<< QObject::tr("Mapping mode") << mappingMode.toString()
				<< QObject::tr("TR069 management") << tr69Management.toString()
				<< QObject::tr("TR069 IP index") << tr69IPIndex.toString();
	}
	virtual ~LineProfileInfo()
	{	}
};

class LineProfileInfoList : public QList<LineProfileInfoBase>
{
public:
	virtual QStringList toStringInfoData() const
	{
		QStringList rtn;
		for( int i = 0; i < count(); i++ )
		{
			rtn << "" << QObject::tr("Line Profile n.%1").arg(i) << at(i).toStringInfoData();
		}
		return rtn;
	}
};

class LineProfile : public OLTCommandReply
{
	LineProfileInfo m_lineProfile;

public:
	LineProfile(const QString &label,const QString &command, const QString &rawData);

protected:
	virtual QStringList toStringInfoData() const
	{
		return m_lineProfile.toStringInfoData();
	}
};

class LineProfiles : public OLTCommandReply
{
	LineProfileInfoList m_lineProfiles;

public:
	LineProfiles(const QString &label,const QString &command, const QString &rawData);

protected:
	virtual QStringList toStringInfoData() const
	{
		return m_lineProfiles.toStringInfoData();
	}
};
}
#endif // OLTCOMMANDS_LINEPROFILE_H
