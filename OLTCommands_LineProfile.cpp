#include "OLTCommands_LineProfile.h"

namespace OLTCommands
{
LineProfile::LineProfile(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{
	QStringList lines = splitLines(rawData);
	QString key, value;

	for( int i = 0; i < lines.count(); i++ )
	{
		splitField(lines[i], key, value);
		if( key.contains("Profile-ID") )			m_lineProfile.id.fromString(value);
		else if( key.contains("Profile-name") )		m_lineProfile.name.fromString(value);
		else if( key.contains("Access-type") )		m_lineProfile.accessType.fromString(value);
		else if( key.contains("FEC upstream") )		m_lineProfile.fecUpstreamSwitch.fromString(value);
		else if( key.contains("OMCC encrypt") )		m_lineProfile.omccEncryptSwitch.fromString(value);
		else if( key.contains("Qos mode") )			m_lineProfile.qosMode.fromString(value);
		else if( key.contains("Mapping mode") )		m_lineProfile.mappingMode.fromString(value);
		else if( key.contains("TR069 management") )	m_lineProfile.tr69Management.fromString(value);
		else if( key.contains("TR069 IP index") )	m_lineProfile.tr69IPIndex.fromString(value);
		else if( key.contains("Binding times") )	m_lineProfile.bindingTimes = value.toInt();
	}
}

LineProfiles::LineProfiles(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{
	QRegExp matcher("\\s+(\\d+)\\s+(\\S+)\\s+(\\d+)\\s*[\\n*\\r*]", Qt::CaseInsensitive);

	int pos = 0;
	LineProfileInfo lineProfile;
	while( (pos = matcher.indexIn(rawData, pos)) != -1 )
	{
		lineProfile.id.fromString( matcher.cap(1) );
		lineProfile.name.fromString( matcher.cap(2) );
		lineProfile.bindingTimes = matcher.cap(3).toInt();
		m_lineProfiles.append(lineProfile);

		pos += matcher.matchedLength();
	}
}
}
