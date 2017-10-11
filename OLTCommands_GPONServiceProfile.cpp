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
	QStringList lines = splitLines(rawData);
#ifndef QT_NO_DEBUG
	QString line;
#endif
	int i;

	for( i = 0; i < lines.count(); i++ )
	{
#ifndef QT_NO_DEBUG
		line = lines[i];
#endif
		if( lines[i].startsWith("  Profile-ID  :") )
			m_srvProfile.id = lines[i].mid(16).trimmed().toInt();
		else if( lines[i].startsWith("  Profile-name:") )
		{
			m_srvProfile.name = lines[i].mid(16).trimmed();
			break;
		}
	}
	while( (++i < lines.count()) && (lines[i] != "  Port-type     Port-number") )
		;
	while( ++i < lines.count() )
	{
		if( lines[i].startsWith("  POTS") )			m_srvProfile.pots = lines[i].mid(16).trimmed();
		else if( lines[i].startsWith("  ETH") )		m_srvProfile.eths = lines[i].mid(16).trimmed();
		else if( lines[i].startsWith("  VDSL") )	m_srvProfile.vdsl = lines[i].mid(16).trimmed();
		else if( lines[i].startsWith("  TDM") )		m_srvProfile.tdm = lines[i].mid(16).trimmed();
		else if( lines[i].startsWith("  MOCA") )	m_srvProfile.moca = lines[i].mid(16).trimmed();
		else if( lines[i].startsWith("  CATV") )
		{
			m_srvProfile.catv = lines[i].mid(16).trimmed();
			break;
		}
	}

	while( (++i < lines.count()) && (lines[i] != "  Port-type Port-ID QinQmode  PriorityPolicy Inbound     Outbound") )
		;
	while( ++i < lines.count() )
	{
		i++;
		// TODO
		break;
	}
	while( (++i < lines.count()) && (lines[i] != "  Port-type Port-ID  DownstreamMode  MismatchPolicy") )
		;
	while( ++i < lines.count() )
	{
		i++;
		// TODO
		break;
	}
	while( (++i < lines.count()) && (lines[i] != "  Port-type Port-ID Dscp-mapping-table-index") )
	{
		i++;
		// TODO
		break;
	}
	while( (++i < lines.count()) && (lines[i] != "  Port   Port Service-type Index S-VLAN S-PRI C-VLAN C-PRI ENCAP      S-PRI") )
		;
	while( ++i < lines.count() )
	{
		i+=2;
		// TODO
		break;
	}
	while( (++i < lines.count()) && (lines[i] != "  Port-type  Port-ID    IGMP-mode         IGMP-VLAN  IGMP-PRI  Max-MAC-Count") )
		;
	while( ++i < lines.count() )
	{
		i++;
		// TODO
		break;
	}
	while( ++i < lines.count() )
	{
		if( lines[i].startsWith("  Binding times :") )
			m_srvProfile.bindedTimes = lines[i].mid(18).trimmed().toInt();
	}
}
