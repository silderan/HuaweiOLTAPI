#include "OLTCommands_ONTVersion.h"

namespace OLTCommands
{
ONTVersion::ONTVersion(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{
	QStringList lines = splitLines(rawData);

	for( int i = 0; i < lines.count(); i++ )
	{
		if( lines[i].startsWith("  F/S/P") )			m_versionInfo.fromString(lines[i].mid(29), 0, 1, 2, -1, -1);
		else if( lines[i].startsWith("  ONT-ID") )		m_versionInfo.id = lines[i].mid(29).toInt();
		else if( lines[i].startsWith("  Vendor") )		m_versionInfo.vendorID = lines[i].mid(29);
		else if( lines[i].startsWith("  ONT Ve") )		m_versionInfo.ontVersion = lines[i].mid(29);
		else if( lines[i].startsWith("  Equipm") )		m_versionInfo.equipmentID = lines[i].mid(29);
		else if( lines[i].startsWith("  Main S") )		m_versionInfo.mainSwVersion = lines[i].mid(29);
		else if( lines[i].startsWith("  Standby") )		m_versionInfo.standbySwVersion = lines[i].mid(29);
		else if( lines[i].startsWith("  Support XML") )	m_versionInfo.suportXMLVersion = lines[i].mid(29);
		else if( lines[i].startsWith("  OntProduct") )
		{
			m_versionInfo.productDesc = lines[i].mid(29);
			if( i++ < lines.count() )
			{
				m_versionInfo.productDesc += lines[i].mid(29);
			}
		}
	}
}
}
