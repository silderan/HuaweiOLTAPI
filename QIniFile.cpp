/*
	Copyright 2015 Rafael Dellà Bort. silderan (at) gmail (dot) com

	This file is part of QMikAPI.

	QMikAPI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as
	published by the Free Software Foundation, either version 3 of
	the License, or (at your option) any later version.

	QMikAPI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	and GNU Lesser General Public License. along with QMikAPI.  If not,
	see <http://www.gnu.org/licenses/>.
 */

#include "QIniFile.h"

#include <QFile>

/**
 * @brief QIniFile::load
 * Loads a ini-like file and places all data into data map.
 * This function only returns false if file cannot be opened to read.
 * If, for any reason, cannot read data correctly, no error is reported.
 * One example where file can be corrupted is if name or value of QIniData
 * contains a newline '/n' or equal character '='
 * @param file filename of ini file.
 * @param data The data mapping to store data readed from file.
 * @return false if cannot open file to read.
 */
bool QIniFile::load(const QString &file, QIniData *data)
{
	QFile f(file);
	if( f.open(QIODevice::ReadOnly) )
	{
		QByteArray linea;
		QList<QByteArray> bits;
		while( !f.atEnd() )
			if( (linea = f.readLine().replace("\n", "")).count() )
				if( (bits = linea.split('=')).count() == 2 )
					(*data)[QString::fromLatin1(bits[0])] = QString::fromLatin1(bits[1]);
		f.close();
		return true;
	}
	return false;
}

/**
 * @brief QIniFile::save
 * Saves data into file in a ini-like format.
 * That is <name>=<value> style.
 * If file cannot be opened or data cannot be writen, "false" is returned.
 * @param file The filename of file to store the data
 * @param data The data mappig to store on file.
 * @return false on error opening file or cannot write all data on file.
 */
bool QIniFile::save(const QString &file, const QIniData &data)
{
	QFile f(file);
	if( f.open(QIODevice::WriteOnly) )
	{
		QMap<QString, QString>::const_iterator i;
		for( i = data.constBegin(); i != data.constEnd(); i++ )
		{
			QByteArray ba = QString("%1=%2\n").arg(i.key(), i.value()).toLatin1();
			if( f.write(ba) != ba.count() )
			{
				f.close();
				return false;
			}
		}
		f.close();
		return true;
	}
	return false;
}
