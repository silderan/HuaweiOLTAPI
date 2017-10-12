#ifndef OLTCOMMANDS_UNMANAGEDOLTS_H
#define OLTCOMMANDS_UNMANAGEDOLTS_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands
{
class UnmanagedONTs : public OLTCommandReply
{
protected:
	struct OntInfo : public OntBasicInfo
	{
		QString password;
		QString loid;
		QString checkcode;
		QString vendorID;
		QString hwVersion;
		QString swVersion;
		QString equipID;
		QString autofindTime;
		void clear()
		{
			OntBasicInfo::clear();
			password.clear();
			loid.clear();
			checkcode.clear();
			vendorID.clear();
			hwVersion.clear();
			swVersion.clear();
			equipID.clear();
			autofindTime.clear();
		}
		QStringList toStringInfoData() const
		{
			return OntBasicInfo::toStringInfoData() + QStringList()
					<< "VendorID" << vendorID
					<< "EquipID" << equipID
					<< "hardware" << hwVersion
					<< "Software" << swVersion
					<< "LOID" << loid
					<< "Checkcode" << checkcode;
		}
	};
	typedef QList<OntInfo> OntInfoList;

private:
	OntInfoList m_unmanagedOnts;

public:
	UnmanagedONTs(const QString &tag,const QString &command, const QString &rawData);

	void clear()
	{
		OLTCommandReply::clear();
		m_unmanagedOnts.clear();
	}

protected:
	virtual QStringList toStringInfoData() const;
};
} // End namespace
#endif // OLTCOMMANDS_UNMANAGEDOLTS_H
