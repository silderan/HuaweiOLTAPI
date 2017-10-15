#include "OLTCommands_DBAProfile.h"

namespace OLTCommands
{
DBAProfile::DBAProfile(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{
	QStringList lines = splitLines(rawData);
	QString key, value;

	for( int i = 0; i < lines.count(); i++ )
	{
		splitField(lines[i], key, value);
		if( key.contains("Profile-ID", Qt::CaseInsensitive) )		m_dbaProfile.id.fromString(value);
		else if( key.contains("Profile-nam", Qt::CaseInsensitive) )	m_dbaProfile.name.fromString(value);
		else if( key.contains("type", Qt::CaseInsensitive) )		m_dbaProfile.type.fromString(value);
		else if( key.contains("Bandwidth ", Qt::CaseInsensitive) )	m_dbaProfile.bandwidthCompensation.fromString(value);
		else if( key.contains("fix", Qt::CaseInsensitive) )			m_dbaProfile.fix.fromString(value);
		else if( key.contains("assure", Qt::CaseInsensitive) )		m_dbaProfile.assured.fromString(value);
		else if( key.contains("max", Qt::CaseInsensitive) )			m_dbaProfile.max.fromString(value);
		else if( key.contains("bind",Qt::CaseInsensitive) )			m_dbaProfile.bindedTimes.fromString(value);
	}
}


DBAProfiles::DBAProfiles(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{
	QRegExp matcher("\\s+(\\d+)\\s+(\\d+)\\s+(\\S+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)[\\n*\\r*]", Qt::CaseInsensitive);

	int pos = 0;
	DBAProfileInfo dbaProfile;
	while( (pos = matcher.indexIn(rawData, pos)) != -1 )
	{
		dbaProfile.id.fromString( matcher.cap(1) );
		dbaProfile.type.fromString( matcher.cap(2) );
		dbaProfile.bandwidthCompensation.fromString( matcher.cap(3) );
		dbaProfile.fix.fromString( matcher.cap(4) );
		dbaProfile.assured.fromString( matcher.cap(5) );
		dbaProfile.max.fromString( matcher.cap(6) );
		dbaProfile.bindedTimes.fromString( matcher.cap(7) );

		m_dbaProfiles.append(dbaProfile);

		pos += matcher.matchedLength();
	}
}
}
