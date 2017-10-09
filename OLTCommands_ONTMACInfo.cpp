#include "OLTCommands_ONTMACInfo.h"

ONTMACInfo::ONTMACInfo(const QString &tag, const QString &command, const QString &rawData) :
	CommandReceivedInfo(tag, command, rawData)
{
	QRegExp matcher("\\n\\s+(\\d+)\\s+([\\w-]+)\\s+(\\w+)\\s+([\\w:-]+)\\s+(\\w+)\\s+(\\d+)[\\s\\/]+(\\d+)[\\s\\/]+(\\d+)[\\s]+(\\d+)[\\s]+(\\d+)[\\s]+(\\d+)",
			Qt::CaseInsensitive);

	int pos = 0;
	WANInfo wanInfo;
	while( (pos = matcher.indexIn(rawData, pos)) != -1 )
	{
		wanInfo.serviceProfile = matcher.cap(1).toInt();
		wanInfo.bundleType = matcher.cap(2);
		wanInfo.portType = matcher.cap(3);
		wanInfo.macAddress = matcher.cap(4);
		wanInfo.macType = matcher.cap(5);
		wanInfo.frame = matcher.cap(6).toInt();
		wanInfo.slot = matcher.cap(7).toInt();
		wanInfo.port = matcher.cap(8).toInt();
		wanInfo.vpi = matcher.cap(9).toInt();
		wanInfo.pci = matcher.cap(10).toInt();
		wanInfo.vlan = matcher.cap(11).toInt();
		m_wanInfo.append(wanInfo);

		pos += matcher.matchedLength();
	}
}
