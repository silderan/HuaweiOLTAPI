#ifndef OLTCommands_GPONSERVICEPROFILE_H
#define OLTCommands_GPONSERVICEPROFILE_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands
{
	class GPONServiceProfile;
	class GPONServiceProfile;
	class ServiceProfilesBase;
	class ServiceProfilesList;
}

struct ServiceProfilesBase
{
	int id;
	QString name;
	int bindedTimes;
	void clear()
	{
		id = 0;
		bindedTimes = 0;
		name.clear();
	}
	ServiceProfilesBase() : id(0), bindedTimes(0) { }
	virtual QStringList toStringInfoData() const
	{
		return QStringList()
				<< QObject::tr("Profile ID")	<< QString::number(id)
				<< QObject::tr("Nombre")		<< name
				<< QObject::tr("Usado")		<< QString("%1 veces").arg(bindedTimes);
	}
	virtual ~ServiceProfilesBase()
	{ }
};

struct ServiceProfile : public ServiceProfilesBase
{
	QString pots;	// Number of POTS (telephony RJ11 plugs)
	QString eths;	// Number of ehternet ports.
	QString vdsl;	// Number of VDSL ports.
	QString tdm;	// Number of TDM ports.
	QString moca;	// Number of MOCA ports.
	QString catv;	// Number of CATV conectors.
	virtual QStringList toStringInfoData() const
	{
		return ServiceProfilesBase::toStringInfoData()
				<< QObject::tr("Puertos telefonía (POTS)") << pots
				<< QObject::tr("Puertos ethernet") << eths
				<< QObject::tr("Puertos VDSL") << vdsl
				<< QObject::tr("Puertos TDM") << tdm
				<< QObject::tr("Puertos MOCA") << moca
				<< QObject::tr("Conectores antena (CATV)") << catv;
	}
};

class ServiceProfilesList : public QList<ServiceProfilesBase>
{
public:
	virtual QStringList toStringInfoData() const
	{
		QStringList rtn;
		for( int i = 0; i < count(); i++ )
		{
			rtn += QStringList() << "" << QObject::tr("Service Profile Nº %1").arg(i)
								 << at(i).toStringInfoData();
		}
		return rtn;
	}
};

class GPONServiceProfiles : public OLTCommandReply
{
protected:
private:
	ServiceProfilesList m_serviceProfileList;

public:
	GPONServiceProfiles(const QString &label, const QString &command, const QString &rawData);
	void clear()
	{
		OLTCommandReply::clear();
		m_serviceProfileList.clear();
	}

protected:
	virtual QStringList toStringInfoData() const
	{
		return QStringList()
				<< "" << QObject::tr("Service Profiles")
				<< m_serviceProfileList.toStringInfoData();
	}
};

class GPONServiceProfile : public OLTCommandReply
{
	ServiceProfile m_srvProfile;

protected:
	virtual QStringList toStringInfoData() const
	{
		return m_srvProfile.toStringInfoData();
	}
public:
	GPONServiceProfile(const QString &label, const QString &command, const QString &rawData);
};

#endif // GPONSERVICEPROFILE_H
