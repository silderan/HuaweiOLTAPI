#ifndef OLTCOMMANDS_DBAPROFILE_H
#define OLTCOMMANDS_DBAPROFILE_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands
{
struct DBAProfileInfo
{
	OLTIntValue id;
	OLTStringValue name;
	OLTIntValue type;
	OLTStringValue bandwidthCompensation;
	OLTIntValue fix;
	OLTIntValue assured;
	OLTIntValue max;
	OLTIntValue bindedTimes;
	void clear()
	{
		id.clear();
		name.clear();
		type.clear();
		bandwidthCompensation.clear();
		fix.clear();
		assured.clear();
		max.clear();
		bindedTimes.clear();
	}
	virtual QStringList toStringInfoData() const
	{
		return QStringList()
				<< QObject::tr("ID") << id.toString()
				<< QObject::tr("Nombre") << name.toString()
				<< QObject::tr("Tipo") << type.toString()
				<< QObject::tr("Compensación de ancho de banda") << bandwidthCompensation.toString()
				<< QObject::tr("Ancho de banda fijo") << fix.toString()
				<< QObject::tr("Ancho de banda asegurado") << assured.toString()
				<< QObject::tr("Ancho de banda máximo") << max.toString()
				<< QObject::tr("Veces usado") << bindedTimes.toString();
	}
	virtual ~DBAProfileInfo()
	{	}
};

class DBAProfileInfoList : public QList<DBAProfileInfo>
{
public:
	virtual QStringList toStringInfoData() const
	{
		QStringList rtn;
		for( int i = 0; i < count(); i++ )
		{
			rtn << "" << QObject::tr("DBA Profile n.%1").arg(i) << at(i).toStringInfoData();
		}
		return rtn;
	}
};

class DBAProfile : public OLTCommandReply
{
	DBAProfileInfo m_dbaProfile;

public:
	DBAProfile(const QString &label,const QString &command, const QString &rawData);

protected:
	virtual QStringList toStringInfoData() const
	{
		return m_dbaProfile.toStringInfoData();
	}
};

class DBAProfiles : public OLTCommandReply
{
	DBAProfileInfoList m_dbaProfiles;

public:
	DBAProfiles(const QString &label,const QString &command, const QString &rawData);

protected:
	virtual QStringList toStringInfoData() const
	{
		return m_dbaProfiles.toStringInfoData();
	}
};
} // End namespace
#endif // OLTCOMMANDS_DBAPROFILE_H
