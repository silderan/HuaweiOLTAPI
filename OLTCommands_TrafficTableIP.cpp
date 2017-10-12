#include "OLTCommands_TrafficTableIP.h"

#include <QRegExp>

namespace OLTCommands
{
TrafficTableIP::TrafficTableIP(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{
	QStringList lines = splitLines(rawData);
	QString cir, pir, cbs, pbs, fix;
	for( int i = 0; i < lines.count(); i++ )
	{
		if( lines[i].startsWith("  TD Index") )				m_trafficTableIPInfo.id = lines[i].mid(25).toInt();
		else if( lines[i].startsWith("  TD Name"))			m_trafficTableIPInfo.name = lines[i].mid(25);
		else if( lines[i].startsWith("  Priority"))			m_trafficTableIPInfo.priority = lines[i].mid(25).toInt();
		else if( lines[i].startsWith("  Copy"))				m_trafficTableIPInfo.copyPolicy = lines[i].mid(25);
		else if( lines[i].startsWith("  Mapping"))			m_trafficTableIPInfo.mappingIndex = lines[i].mid(25);
		else if( lines[i].startsWith("  CTAG Mapping Pri"))	m_trafficTableIPInfo.ctagMappingIndex = lines[i].mid(25);
		else if( lines[i].startsWith("  CTAG Mapping Ind"))	m_trafficTableIPInfo.ctagMappingIndex = lines[i].mid(25);
		else if( lines[i].startsWith("  CTAG Default Pri"))	m_trafficTableIPInfo.ctagDefaultPriority = lines[i].mid(25);
		else if( lines[i].startsWith("  Priority Policy"))	m_trafficTableIPInfo.priorityPolicy = lines[i].mid(25);
		else if( lines[i].startsWith("  CIR"))				cir = lines[i].mid(25);
		else if( lines[i].startsWith("  CBS"))				cbs = lines[i].mid(25);
		else if( lines[i].startsWith("  PIR"))				pir = lines[i].mid(25);
		else if( lines[i].startsWith("  PBS"))				pbs = lines[i].mid(25);
		else if( lines[i].startsWith("  Fix")			)	fix = lines[i].mid(25);
		else if( lines[i].startsWith("  CAR Threshold"))	m_trafficTableIPInfo.carThresholdProfile = lines[i].mid(25);
		else if( lines[i].startsWith("  Color Mode"))		m_trafficTableIPInfo.colorMode = lines[i].mid(25);
		else if( lines[i].startsWith("  Color policy"))		m_trafficTableIPInfo.colorPolicy = lines[i].mid(25);
		else if( lines[i].startsWith("  Referenced Status"))m_trafficTableIPInfo.referencedStatus = lines[i].mid(25);
	}
	QRegExp number("(\\d+)");
	if( number.indexIn(cir) != -1 ) m_trafficTableIPInfo.cir = number.cap(1).toInt();
	if( number.indexIn(pir) != -1 ) m_trafficTableIPInfo.pir = number.cap(1).toInt();
	if( number.indexIn(cbs) != -1 ) m_trafficTableIPInfo.cbs = number.cap(1).toInt();
	if( number.indexIn(pbs) != -1 ) m_trafficTableIPInfo.pbs = number.cap(1).toInt();
	if( number.indexIn(fix) != -1 ) m_trafficTableIPInfo.fix = number.cap(1).toInt();
}


TrafficTableIPs::TrafficTableIPs(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{
	QRegExp matcher("\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\S+)\\s+(\\S+)[\\n|\\r]",
			Qt::CaseInsensitive);

	int pos = 0;
	TrafficTableBasicIPInfo trafficInfo;
	while( (pos = matcher.indexIn(rawData, pos)) != -1 )
	{
		trafficInfo.id = matcher.cap(1).toInt();
		trafficInfo.cir = matcher.cap(2).toInt();
		trafficInfo.cbs = matcher.cap(3).toInt();
		trafficInfo.pir = matcher.cap(4).toInt();
		trafficInfo.pbs = matcher.cap(5).toInt();
		trafficInfo.priority = matcher.cap(6).toInt();
		trafficInfo.copyPolicy = matcher.cap(7);
		trafficInfo.priorityPolicy = matcher.cap(8).toInt();

		m_trafficTableIPList.append(trafficInfo);

		pos += matcher.matchedLength();
	}
}
}
