#ifndef OLTCOMMANDS_ONTMACINFO_H
#define OLTCOMMANDS_ONTMACINFO_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands
{
	class ONTMACInfo;
}

class ONTMACInfo : public OLTCommandReply
{
protected:
	struct WANInfo : public OntBasicInfo
	{
		int serviceProfile;
		QString bundleType;
		QString portType;
		QString macAddress;
		QString macType;
		int vpi;
		int pci;
		quint16 vlan;
		void clean()
		{
			serviceProfile = -1;
			bundleType.clear();
			portType.clear();
			macAddress.clear();
			macType.clear();
			vpi = -1;
			pci = -1;
			vlan = 0;
		}
		QStringList toStringInfoData() const
		{
			return OntBasicInfo::toStringInfoData()
					<< QObject::tr("service-profile-index")	<< QString::number(serviceProfile)
					<< QObject::tr("Bundle Type")			<< bundleType
					<< QObject::tr("Tipo puerto")			<< portType
					<< QObject::tr("Dirección MAC")			<< macAddress
					<< QObject::tr("Tipo MAC")				<< macType
					<< QObject::tr("VPI")					<< QString::number(vpi)
					<< QObject::tr("PCI")					<< QString::number(pci)
					<< QObject::tr("VLAN")					<< QString::number(vlan);
		}
	};
	class WANInfoList : public QList<WANInfo>
	{
	public:
		QStringList toStringInfoData() const
		{
			QStringList rtn;
			for( int i = 0; i < count(); i++ )
			{
				rtn += QStringList() << "" << QObject::tr("WAN Interface Nº %1").arg(i)
									 << at(i).toStringInfoData();
			}
			return rtn;
		}
	};

private:
	WANInfoList m_wanInfo;

public:
	ONTMACInfo(const QString &tag,const QString &command, const QString &rawData);

	void clear()
	{
		OLTCommandReply::clear();
		m_wanInfo.clear();
	}
protected:
	virtual QStringList toStringInfoData(bool includeRaw) const
	{
		Q_UNUSED(includeRaw);
		return OLTCommandReply::toStringInfoData(false)
				<< "" << QObject::tr("Interficies WAN")
				<< m_wanInfo.toStringInfoData();
	}
};

#endif // ONTMACINFO_H
