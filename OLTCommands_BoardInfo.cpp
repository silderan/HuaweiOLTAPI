#include "OLTCommands_BoardInfo.h"

BoardInfo::BoardInfo(const QString &tag, const QString &command, const QString &rawData)
	: CommandReceivedInfo(tag, command, rawData)
{
	QStringList lines = splitLines(rawData);
	int i = 0;
	while( i < lines.count() )
	{
		if( lines[i].contains("Board Name") )
			m_boardName = lines[i].mid(24).trimmed();
		else
		if( lines[i].contains("Board Status") )
			m_boardStatus = lines[i].mid(24).trimmed();
		else
		if( lines[i].contains("Port   Port   min-distance   max-distance   Optical-module") )
			break;
		i++;
	}
	PortInfo portInfo;
	bool converted;
	while( i < lines.count() )
	{
		if( lines[i].contains("F/S/P   ONT         SN         Control     Run      Config   Match    Protect") )
			break;
		lines[i].left(7).toInt(&converted);
		if( converted )
		{
			portInfo.portType = lines[i].mid(19, 9).trimmed();
			portInfo.minDistance = lines[i].mid(21, 6).trimmed().toInt();
			portInfo.maxDistance = lines[i].mid(36, 6).trimmed().toInt();
			portInfo.opticalStatus = lines[i].mid(49).trimmed();
			m_portInfoList.append(portInfo);
			portInfo.clear();
		}
		i++;
	}
	OntInfo ontInfo;
	while( i < lines.count() )
	{
		QStringList resto = ontInfo.fromString(lines[i], 0, 1, 2, 3, 4);
		if( resto.count() )
		{
			if( resto.count() == 5 )
			{
				ontInfo.controlFlag = resto[0];
				ontInfo.runState = resto[1];
				ontInfo.configState = resto[2];
				ontInfo.matchState = resto[3];
				ontInfo.protectSide = resto[4];
				m_ontInfoList.append(ontInfo);
				ontInfo.clear();
			}
		}
		else
		if( (resto = ontInfo.fromString(lines[i], 0, 1, 2, 3, -1)).count() )
		{
			for( int o = 0; o < m_ontInfoList.count(); o++ )
			{
				if( (m_ontInfoList[o].frame == ontInfo.frame) &&
					(m_ontInfoList[o].slot == ontInfo.slot) &&
					(m_ontInfoList[o].port == ontInfo.port) &&
					(m_ontInfoList[o].id == ontInfo.id) )
				{
					m_ontInfoList[o].descript = resto.join(" ");
				}
			}
		}
		i++;
	}
}

QStringList BoardInfo::toStringInfoData(bool includeRaw) const
{
	Q_UNUSED(includeRaw);
	QStringList rtn = CommandReceivedInfo::toStringInfoData(false);
	rtn.append("Board Name"); rtn.append(m_boardName);
	rtn.append("Board Status"); rtn.append(m_boardStatus);
	int i;
	for( i = 0; i < m_portInfoList.count(); i++ )
	{
		rtn.append( QString("Port %1").arg(i) );
		rtn.append( QString("%2, %3-%4 (%5)")
					.arg(m_portInfoList[i].portType)
					.arg(m_portInfoList[i].minDistance)
					.arg(m_portInfoList[i].maxDistance)
					.arg(m_portInfoList[i].opticalStatus) );
	}
	for( i = 0; i < m_ontInfoList.count(); i++ )
	{
		rtn.append( QString("Ont %1").arg(i) );
		rtn.append( QString("%1/%2/%3-%4 %5 %6 %7 %8 %9 %10 %11")
					.arg(m_ontInfoList[i].frame)
					.arg(m_ontInfoList[i].slot)
					.arg(m_ontInfoList[i].port)
					.arg(m_ontInfoList[i].id)
					.arg(m_ontInfoList[i].serial)
					.arg(m_ontInfoList[i].controlFlag)
					.arg(m_ontInfoList[i].runState)
					.arg(m_ontInfoList[i].configState)
					.arg(m_ontInfoList[i].matchState)
					.arg(m_ontInfoList[i].protectSide)
					.arg(m_ontInfoList[i].descript) );
	}
	return rtn;
}
