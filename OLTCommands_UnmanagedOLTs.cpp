#include "OLTCommands_UnmanagedOLTs.h"

UnmanagedONTs::UnmanagedONTs(const QString &tag, const QString &command, const QString &rawData) :
	CommandReceivedInfo(tag, command, rawData)
{
	QStringList pair;
	QStringList lines = splitLines(rawData);
	OntInfo ont;
	for( int l = 0; l < lines.count(); l++ )
	{
		if( (pair = lines[l].split(QRegExp("\\s+:\\s+"), QString::SkipEmptyParts)).count() != 2 )
			continue;
		if( pair[0].contains("F/S/P") )
			ont.fromString( pair[1], 0, 1, 2, -1, -1 );
		else if( pair[0].contains("Ont SN") )
			ont.serial = pair[1].trimmed();
		else if( pair[0].contains("Password") )
			ont.password = pair[1].trimmed();
		else if( pair[0].contains("Loid") )
			ont.loid = pair[1].trimmed();
		else if( pair[0].contains("Checkcode") )
			ont.checkcode = pair[1].trimmed();
		else if( pair[0].contains("VendorID") )
			ont.vendorID = pair[1].trimmed();
		else if( pair[0].contains("Ont Version") )
			ont.hwVersion = pair[1].trimmed();
		else if( pair[0].contains("Ont SoftwareVersion") )
			ont.swVersion = pair[1].trimmed();
		else if( pair[0].contains("Ont EquipmentID") )
			ont.equipID = pair[1].trimmed();
		else if( pair[0].contains("Ont autofind time") )
		{
			m_unmanagedOnts.append(ont);
			ont.clear();
		}
	}
}

QStringList UnmanagedONTs::toStringInfoData(bool includeRaw) const
{
	Q_UNUSED(includeRaw);
	QStringList rtn = CommandReceivedInfo::toStringInfoData();
	for( int ont = 0; ont < m_unmanagedOnts.count(); ont++ )
	{
		rtn += QStringList() << "Unmanaged ONT Number" << QString::number(ont);
		rtn += m_unmanagedOnts[ont].toStringInfoData();
	}
	return rtn;
}
