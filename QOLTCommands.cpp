#include "QOLTCommands.h"

#include <QFile>

void QOLTCommands::Command::fromFileLine(const QByteArray &line)
{
	if( !line.startsWith('#') )
	{
		int pos = line.indexOf('=');
		if( pos != -1 )
			insert(line.left(pos).trimmed(), line.mid(pos).trimmed());
	}
}

QString QOLTCommands::ErrorStrings::errorString(const QString &text) const
{
	QOLTCommands::ErrorStrings::const_iterator i = constBegin();
	while( i != constEnd() )
	{
		if( text.contains(i.key()) )
			return i.value();
		i++;
	}
	// No error.
	return QString();
}

QOLTCommands::QOLTCommands()
{
	loadCommandFile();
}

void QOLTCommands::loadCommandFile()
{
	QFile f("OLTCommands.cnf");
	if( f.open(QIODevice::ReadOnly) )
	{
		QByteArray linea;
		QList<QByteArray> bits;

		while( !f.atEnd() )
			if( (linea = f.readLine().replace("\n", "")).count() )
			{
				if( bits[0].startsWith("COMMAND_ERROR:") )
					m_cmdErrors.fromFileLine( linea.mid(14) );
				else
				if( bits[0].startsWith("LOGIN_ERROR:") )
					m_loginErrors.fromFileLine( linea.mid(12) );
				else
				if( (bits = linea.split('=')).count() == 2 )
					m_oltCommands.fromFileLine( linea );
			}
		f.close();
	}
}
