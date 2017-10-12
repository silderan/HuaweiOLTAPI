#ifndef OLTCOMMANDS_ONTWANINFO_H
#define OLTCOMMANDS_ONTWANINFO_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands
{
class ONTWANInfo : public OLTCommandReply
{
protected:
	struct WANInfo
	{
		QString name;
		QString serviceType;
		QString connectionType;
		QString ipv4Status;
		QString ipv4AccessType;//           : DHCP
		QString ipv4Address;//               : 10.50.219.49
		QString ipv4SubnetMask;//                : 255.255.0.0
		QString ipv4DefaulGateway;//            : 10.50.0.1
		int manageVLAN;//                : 3333
		int managePriority;//            : 5
		QString option60;//                   : Yes
		QString hwSwitch;//                     : Enable
		QString MAC;//                : 48FD-8E8F-F405
		QString priorityPolicy;//            : Specified
		QString l2EncapType;//              : IPoE
		QString ipv4Switch;//                : Enable
		QString ipv6Switch;//                : Disable
		QString prefix;//                     : -
		QString prefixAccessMode;//         : Invalid
		QString prefixPreferredTime;//      : -
		QString prefixValidTime;//          : -
		QString ipv6ConnectionStatus;//    : Invalid
		QString ipv6Address;//               : -
		QString ipv6AddressStatus;//        : Invalid
		QString ipv6AddressAccessMode;//   : Invalid
		QString ipv6AddressPreferredTime;//: -
		QString ipv6AddressValidTime;//    : -
		QString dsLiteMode;//               : Invalid
		QString dsLitePeerAddress;//       : -
		void clear()
		{
			name.clear();
			serviceType.clear();
			connectionType.clear();
			ipv4Status.clear();
			ipv4AccessType.clear();
			ipv4Address.clear();
			ipv4SubnetMask.clear();
			ipv4DefaulGateway.clear();
			manageVLAN = -1;
			managePriority = -1;
			option60.clear();
			hwSwitch.clear();
			MAC.clear();
			priorityPolicy.clear();
			l2EncapType.clear();
			ipv4Switch.clear();
			ipv6Switch.clear();
			prefix.clear();
			prefixAccessMode.clear();
			prefixPreferredTime.clear();
			prefixValidTime.clear();
			ipv6ConnectionStatus.clear();
			ipv6Address.clear();
			ipv6AddressStatus.clear();
			ipv6AddressAccessMode.clear();
			ipv6AddressPreferredTime.clear();
			dsLiteMode.clear();
			dsLitePeerAddress.clear();
		}
	};
	typedef QList<WANInfo> WANInfoList;

	struct OntInfo : public OntBasicInfo
	{
		WANInfoList wanInfoList;
		void clean()
		{
			OntBasicInfo::clear();
			wanInfoList.clear();
		}
	};

private:
	OntInfo m_ontInfo;

public:
	ONTWANInfo(const QString &tag,const QString &command, const QString &rawData);

	void clear()
	{
		OLTCommandReply::clear();
		m_ontInfo.clear();
	}
protected:
	virtual QStringList toStringInfoData() const;
};
} // End namespace
#endif // OLTCOMMANDS_ONTWANINFO_H
