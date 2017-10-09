#include "OLTCommands_BaseInfo.h"

#include <QRegExp>

QStringList OLTCommandReply::OntBasicInfo::fromString(const QString &txt, int framePos, int slotPos, int portPos, int idPos, int serialPos)
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

QStringList OLTCommandReply::splitLines(const QString &txt)
{
	return txt.split(QRegExp("[\\n\\r]+"), QString::SkipEmptyParts);
}

int OLTCommandReply::splitField(const QString &line, QString &key, QString &value)
{
	int split = line.indexOf(':');
	if( split != -1 )
	{
		key = line.left(split).trimmed();
		value = line.mid(split+1).trimmed();
		return 2;
	}
	else
	{
		key = line;
		value.clear();
		return 1;
	}
}

QString OLTCommandReply::toString() const
{
	QStringList info = toStringInfoData(true);
	QString rtn;
	int i;
	int keyLength = 0;
	int valueLength = 0;
	for( i = 0; i < info.count(); i+= 2 )
	{
		if( keyLength < info[i+0].length() )
			keyLength = info[i+0].length();
		if( valueLength < info[i+1].length() )
			valueLength = info[i+1].length();
	}

	keyLength++;
	for( i = 0; i < info.count(); i+= 2 )
	{
		if( info[i+0].count() )
			rtn += QString("%1 : %2\n").arg(info[i+0], keyLength, ' ').arg(info[i+1]);
		else
		{
			int center = info[i+1].count();
			QString laterals = QString("_").repeated(((valueLength+keyLength+3)>>1) - (center>>1));
			rtn += QString("%1%2%3\n").arg(laterals, info[i+1], laterals);
		}
	}
	return rtn;
}
