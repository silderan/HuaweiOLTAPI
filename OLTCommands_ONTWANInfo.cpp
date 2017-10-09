#include "OLTCommands_ONTWANInfo.h"

ONTWANInfo::ONTWANInfo(const QString &tag, const QString &command, const QString &rawData) :
	OLTCommandReply(tag, command, rawData)
{
	QStringList lines = splitLines(rawData);
	QString key, value;
	WANInfo wanInfo;
	for( int i = 0; i < lines.count(); i++ )
	{
		splitField(lines[i], key, value);
		if( key == "F/S/P" )			m_ontInfo.fromString(value, 0, 1, 2, -1, -1);
		else if( key == "ONT ID" )		m_ontInfo.id = value.toInt();
		else if( key == "Name")							wanInfo.name = value;
		else if( key == "Service type")					wanInfo.serviceType = value;
		else if( key == "Connection type")				wanInfo.connectionType = value;
		else if( key == "IPv4 Connection status")		wanInfo.ipv4Status = value;
		else if( key == "IPv4 access type")				wanInfo.ipv4AccessType = value;
		else if( key == "IPv4 address")					wanInfo.ipv4Address = value;
		else if( key == "Subnet mask")					wanInfo.ipv4SubnetMask = value;
		else if( key == "Default gateway")				wanInfo.ipv4DefaulGateway = value;
		else if( key == "Manage VLAN")					wanInfo.manageVLAN = value.toInt();
		else if( key == "Manage priority")				wanInfo.managePriority = value.toInt();
		else if( key == "Option60")						wanInfo.option60 = value;
		else if( key == "Switch")						wanInfo.hwSwitch = value;
		else if( key == "MAC address")					wanInfo.MAC = value;
		else if( key == "Priority policy")				wanInfo.priorityPolicy = value;
		else if( key == "L2 encap-type")				wanInfo.l2EncapType = value;
		else if( key == "IPv4 switch")					wanInfo.ipv4Switch = value;
		else if( key == "IPv6 switch")					wanInfo.ipv6Switch = value;
		else if( key == "Prefix")						wanInfo.prefix = value;
		else if( key == "Prefix access mode")			wanInfo.prefixAccessMode = value;
		else if( key == "Prefix preferred time")		wanInfo.prefixPreferredTime = value;
		else if( key == "Prefix valid time")			wanInfo.prefixValidTime = value;
		else if( key == "IPv6 Connection status")		wanInfo.ipv6ConnectionStatus = value;
		else if( key == "IPv6 address")					wanInfo.ipv6Address = value;
		else if( key == "IPv6 address status")			wanInfo.ipv6AddressStatus = value;
		else if( key == "IPv6 address access mode")		wanInfo.ipv6AddressAccessMode = value;
		else if( key == "IPv6 address preferred time")	wanInfo.ipv6AddressPreferredTime = value;
		else if( key == "IPv6 address valid time")		wanInfo.ipv6AddressValidTime = value;
		else if( key == "DS-Lite Mode")					wanInfo.dsLiteMode = value;
		else if( key == "DS-Lite peer address")
		{
			wanInfo.dsLitePeerAddress = value;
			m_ontInfo.wanInfoList.append(wanInfo);
			wanInfo.clear();
		}
	}
}

QStringList ONTWANInfo::toStringInfoData(bool includeRaw) const
{
	Q_UNUSED(includeRaw);
	QStringList rtn = OLTCommandReply::toStringInfoData(false);
	for( int wan = 0; wan < m_ontInfo.wanInfoList.count(); wan++ )
		rtn += QStringList()
			<< "" << QObject::tr("Interficie WAN número %1").arg(wan)
			<< QObject::tr("Nombre interficie")					<< m_ontInfo.wanInfoList[wan].name
			<< QObject::tr("Tipo de servicio")					<< m_ontInfo.wanInfoList[wan].serviceType
			<< QObject::tr("Tipo de conexión")					<< m_ontInfo.wanInfoList[wan].connectionType
			<< QObject::tr("Estado conexión IPv4")				<< m_ontInfo.wanInfoList[wan].ipv4Status
			<< QObject::tr("Tipo de acceso para IPv4")			<< m_ontInfo.wanInfoList[wan].ipv4AccessType
			<< QObject::tr("Dirección IPV4")					<< m_ontInfo.wanInfoList[wan].ipv4Address
			<< QObject::tr("Máscara red")						<< m_ontInfo.wanInfoList[wan].ipv4SubnetMask
			<< QObject::tr("Puerta de enlace para IPv4")		<< m_ontInfo.wanInfoList[wan].ipv4DefaulGateway
			<< QObject::tr("VLAN")								<< QString::number(m_ontInfo.wanInfoList[wan].manageVLAN)
			<< QObject::tr("Prioridad")							<< QString::number(m_ontInfo.wanInfoList[wan].managePriority)
			<< QObject::tr("Opción 60 del DHCP")				<< m_ontInfo.wanInfoList[wan].option60
			<< QObject::tr("Switch")							<< m_ontInfo.wanInfoList[wan].hwSwitch
			<< QObject::tr("Dirección MAC")						<< m_ontInfo.wanInfoList[wan].MAC
			<< QObject::tr("Política de prioridad")				<< m_ontInfo.wanInfoList[wan].priorityPolicy
			<< QObject::tr("Tipo de encapsulamento capa 2")		<< m_ontInfo.wanInfoList[wan].l2EncapType
			<< QObject::tr("Switch IPv4")						<< m_ontInfo.wanInfoList[wan].ipv4Switch
			<< QObject::tr("switch IPv6")						<< m_ontInfo.wanInfoList[wan].ipv6Switch
			<< QObject::tr("Prefix")							<< m_ontInfo.wanInfoList[wan].prefix
			<< QObject::tr("Prefix access mode")				<< m_ontInfo.wanInfoList[wan].prefixAccessMode
			<< QObject::tr("Prefix preferred time")				<< m_ontInfo.wanInfoList[wan].prefixAccessMode
			<< QObject::tr("Prefix valid time")					<< m_ontInfo.wanInfoList[wan].prefixValidTime
			<< QObject::tr("Estado conexión IPv6")				<< m_ontInfo.wanInfoList[wan].ipv6ConnectionStatus
			<< QObject::tr("Dirección IPv6")					<< m_ontInfo.wanInfoList[wan].ipv6Address
			<< QObject::tr("Estado dirección IPv6")				<< m_ontInfo.wanInfoList[wan].ipv6AddressStatus
			<< QObject::tr("Modo acceso dirección IPv6")		<< m_ontInfo.wanInfoList[wan].ipv6AddressAccessMode
			<< QObject::tr("Tiempo preferido dirección IPv6")	<< m_ontInfo.wanInfoList[wan].ipv6AddressPreferredTime
			<< QObject::tr("Tiempo válido dirección IPv6")		<< m_ontInfo.wanInfoList[wan].ipv6AddressValidTime
			<< QObject::tr("Modo DS-Lite")						<< m_ontInfo.wanInfoList[wan].dsLiteMode
			<< QObject::tr("Dirección peer DS-Lite")			<< m_ontInfo.wanInfoList[wan].dsLitePeerAddress;
	return rtn;
}
