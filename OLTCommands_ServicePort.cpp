#include "OLTCommands_ServicePort.h"

namespace OLTCommands
{
ServicePort::ServicePort(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{
	QStringList lines = splitLines(rawData);
	QString key, value;

	for( int i = 0; i < lines.count(); i++ )
	{
		splitField(lines[i], key, value);
		if( key.contains("Index") )						m_srvPort.index.fromString(value);
		else if( key.contains("VLAN ID") )				m_srvPort.vlan.fromString(value);
		else if( key.contains("VLAN attr") )			m_srvPort.vlanAttrib.fromString(value);
		else if( key.contains("Port type") )			m_srvPort.portType.fromString(value);
		else if( key.contains("F/S/P") )				m_srvPort.fromString(value, 0, 1, 2, -1, -1);
		else if( key.contains("ONT ID") )				m_srvPort.id = value.toInt();
		else if( key.contains("GEM port index") )		m_srvPort.gemportIndex.fromString(value);
		else if( key.contains("Flow type") )			m_srvPort.flow.type.fromString(value);
		else if( key.contains("Flow para") )			m_srvPort.flow.parameter.fromString(value);
		else if( key.contains("TX") )					m_srvPort.txTrafficTableIndex.fromString(value);
		else if( key.contains("Inbound table name") )	m_srvPort.txTrafficTableName.fromString(value);
		else if( key.contains("RX") )					m_srvPort.rxTrafficTableIndex.fromString(value);
		else if( key.contains("Outbound table name") )	m_srvPort.rxTrafficTableName.fromString(value);
		else if( key.contains("Admin status") )			m_srvPort.adminStatus.fromString(value);
		else if( key.contains("State") )				m_srvPort.state.fromString(value);
		else if( key.contains("Label") )				m_srvPort.label.fromString(value);
		else if( key.contains("Priority") )				m_srvPort.priority.fromString(value);
		else if( key.contains("PVC bundle") )			m_srvPort.pvcBundle.fromString(value);
		else if( key.contains("Max MAC count") )		m_srvPort.maxMACCount.fromString(value);
		else if( key.contains("Tag transform") )		m_srvPort.tagTransform.fromString(value);
		else if( key.contains("Description") )			m_srvPort.descrip.fromString(value);
		else if( key.contains("Remote description") )	m_srvPort.remoteDescrip.fromString(value);
		else if( key.contains("Service-port bundle") )	m_srvPort.srvPortBundle.fromString(value);
		else if( key.contains("Cos") )					m_srvPort.cos.fromString(value);
		else if( key.contains("Car-Group") )			m_srvPort.carGroup.fromString(value);
		else if( key.contains("Static MAC") )			m_srvPort.staticMAC.fromString(value);
		else if( key.contains("IP address") )			m_srvPort.ipAddress.fromString(value);
		else if( key.contains("Stat. performance") )	m_srvPort.statPerformance.fromString(value);
	}
}

ServicePorts::ServicePorts(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{
	QRegExp matcher("\\s+(\\d+)\\s+(\\d+)\\s+(\\S+)\\s+(\\S+)\\s+(\\d+)\\/([\\d ]+)\\/(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\S+)\\s+(\\S+)\\s+(\\d+)\\s+(\\d+)\\s+(\\S+)[\\n\\r]+",
			Qt::CaseInsensitive);

	int pos = 0;
	ServicePortInfoBase srvPort;
	while( (pos = matcher.indexIn(rawData, pos)) != -1 )
	{
		srvPort.index.fromString( matcher.cap(1) );
		srvPort.vlan.fromString( matcher.cap(2) );
		srvPort.vlanAttrib.fromString( matcher.cap(3) );
		srvPort.portType.fromString( matcher.cap(4) );
		srvPort.frame = matcher.cap(5).toInt();
		srvPort.slot = matcher.cap(6).toInt();
		srvPort.port = matcher.cap(7).toInt();
		srvPort.id = matcher.cap(8).toInt();
		srvPort.gemportIndex.fromString( matcher.cap(9) );
		srvPort.flow.type.fromString( matcher.cap(10) );
		srvPort.flow.parameter.fromString( matcher.cap(11) );
		srvPort.rxTrafficTableIndex.fromString( matcher.cap(12) );
		srvPort.txTrafficTableIndex.fromString( matcher.cap(13) );
		srvPort.state.fromString( matcher.cap(14) );
		m_srvPorts.append(srvPort);

		pos += matcher.matchedLength();
	}
}
}
