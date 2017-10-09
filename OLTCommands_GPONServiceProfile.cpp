#include "OLTCommands_GPONServiceProfile.h"

GPONServiceProfiles::GPONServiceProfiles(const QString &tag, const QString &command, const QString &rawData) :
	CommandReceivedInfo(tag, command, rawData)
{
	QStringList lines = splitLines(rawData);
	ServiceProfilesBase srvProfileBase;
	bool converted;
	foreach( QString line, lines )
	{
		srvProfileBase.id = line.left(14).trimmed().toInt(&converted);
		if( converted )
		{
			srvProfileBase.name = line.mid(15,42).trimmed();
			srvProfileBase.bindedTimes = line.mid(57).trimmed().toInt();
			m_serviceProfileList.append(srvProfileBase);
		}
	}
}
