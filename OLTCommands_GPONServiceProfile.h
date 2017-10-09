#ifndef OLTCommands_GPONSERVICEPROFILE_H
#define OLTCommands_GPONSERVICEPROFILE_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands
{
	class GPONServiceProfile;
}

class GPONServiceProfiles : public OLTCommandReply
{
protected:
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
private:
	ServiceProfilesList m_serviceProfileList;

public:
	GPONServiceProfiles(const QString &tag,const QString &command, const QString &rawData);
	void clear()
	{
		OLTCommandReply::clear();
		m_serviceProfileList.clear();
	}

protected:
	virtual QStringList toStringInfoData(bool includeRaw) const
	{
		Q_UNUSED(includeRaw);
		return OLTCommandReply::toStringInfoData(false)
				<< "" << QObject::tr("Service Profiles")
				<< m_serviceProfileList.toStringInfoData();
	}
};

#endif // GPONSERVICEPROFILE_H
