#include "OLTCommands_BaseInfo.h"

#include <QRegExp>

QStringList CommandReceivedInfo::OntBasicInfo::fromString(const QString &txt, int framePos, int slotPos, int portPos, int idPos, int serialPos)
{
	QStringList bits = txt.split(QRegExp("[\\s\\/]+"), QString::SkipEmptyParts);
	int i = 0;
	bool converted;

	if( framePos != -1 )
	{
		if( framePos >= bits.count() )
			return QStringList();
		frame = bits[framePos].toInt(&converted);
		if( !converted )
			return QStringList();
		i++;
	}
	if( slotPos != -1 )
	{
		if( slotPos >= bits.count() )
			return QStringList();
		slot = bits[slotPos].toInt(&converted);
		if( !converted )
			return QStringList();
		i++;
	}
	if( portPos != -1 )
	{
		if( portPos >= bits.count() )
			return QStringList();
		port = bits[portPos].toInt(&converted);
		if( !converted )
			return QStringList();
		i++;
	}
	if( idPos != -1 )
	{
		if( idPos >= bits.count() )
			return QStringList();
		id = bits[idPos].toInt(&converted);
		if( !converted )
			return QStringList();
		i++;
	}
	if( serialPos != -1 )
	{
		if( serialPos >= bits.count() )
			return QStringList();
		serial = bits[serialPos];
		i++;
	}
	return bits.mid(i);
}

QStringList CommandReceivedInfo::splitLines(const QString &txt)
{
	return txt.split(QRegExp("[\\n\\r]+"), QString::SkipEmptyParts);
}

QString CommandReceivedInfo::toString() const
{
	QStringList info = toStringInfoData(true);
	QString rtn;
	int i;
	int keyLength;
	for( i = 0; i < info.count(); i+= 2 )
		if( keyLength < info[i].length() )
			keyLength = info[i].length();

	keyLength++;
	for( i = 0; i < info.count(); i+= 2 )
		rtn += QString("%1 : %2\n").arg(info[i+0], keyLength, ' ').arg(info[i+1]);
	return rtn;
}
