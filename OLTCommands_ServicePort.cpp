#include "OLTCommands_ServicePort.h"

namespace OLTCommands
{
ServicePort::ServicePort(const QString &label, const QString &command, const QString &rawData) :
	OLTCommandReply(label, command, rawData)
{

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
		srvPort.index = matcher.cap(1).toInt();
		srvPort.vlan = matcher.cap(2).toInt();
		srvPort.vlanAttrib = matcher.cap(3);
		srvPort.portType.fromString( matcher.cap(4) );
		srvPort.frame = matcher.cap(5).toInt();
		srvPort.slot = matcher.cap(6).toInt();
		srvPort.port = matcher.cap(7).toInt();
		srvPort.vpi = matcher.cap(8).toInt();
		srvPort.vci = matcher.cap(9).toInt();
		srvPort.flow.type = matcher.cap(10);
		srvPort.flow.parameter = matcher.cap(11);
		srvPort.rxTrafficTableIndex = matcher.cap(12).toInt();
		srvPort.txTrafficTableIndex = matcher.cap(13).toInt();
		srvPort.state.fromString( matcher.cap(14) );
		m_srvPorts.append(srvPort);

		pos += matcher.matchedLength();
	}
}
}
