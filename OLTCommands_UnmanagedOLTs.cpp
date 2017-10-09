#include "OLTCommands_UnmanagedOLTs.h"

UnmanagedONTs::UnmanagedONTs(const QString &tag, const QString &command, const QString &rawData) :
	OLTCommandReply(tag, command, rawData)
{
	QString key, value;
	QStringList lines = splitLines(rawData);
	OntInfo ont;
	for( int i = 0; i < lines.count(); i++ )
	{
		splitField(lines[i], key, value);
		if( key.contains("F/S/P") )						ont.fromString( value, 0, 1, 2, -1, -1 );
		else if( key.contains("Ont SN") )				ont.serial = value;
		else if( key.contains("Password") )				ont.password = value;
		else if( key.contains("Loid") )					ont.loid = value;
		else if( key.contains("Checkcode") )			ont.checkcode = value;
		else if( key.contains("VendorID") )				ont.vendorID = value;
		else if( key.contains("Ont Version") )			ont.hwVersion = value;
		else if( key.contains("Ont SoftwareVersion") )	ont.swVersion = value;
		else if( key.contains("Ont EquipmentID") )		ont.equipID = value;
		else if( key.contains("Ont autofind time") )
		{
			m_unmanagedOnts.append(ont);
			ont.clear();
		}
	}
}

QStringList UnmanagedONTs::toStringInfoData() const
{
	QStringList rtn;
	for( int ont = 0; ont < m_unmanagedOnts.count(); ont++ )
	{
		rtn += QStringList() << "Unmanaged ONT Number" << QString::number(ont);
		rtn += m_unmanagedOnts[ont].toStringInfoData();
	}
	return rtn;
}
