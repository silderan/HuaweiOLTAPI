#include "OLTCommands_GPONServiceProfile.h"

GPONServiceProfiles::GPONServiceProfiles(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
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


GPONServiceProfile::GPONServiceProfile(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{

}
