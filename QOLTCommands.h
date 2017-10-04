#ifndef QOLTCOMMANDS_H
#define QOLTCOMMANDS_H

#include <QMap>
#include <qstring.h>

class QOLTCommands
{
public:
	class Command : public QMap<QString, QString>
	{
	public:
		void fromFileLine(const QByteArray &line);
	};

	class ErrorStrings : public Command
	{
	public:
		QString errorString(const QString &text) const;
	};

private:
	Command m_oltCommands;
	ErrorStrings m_cmdErrors;
	ErrorStrings m_loginErrors;
	void loadCommandFile();

public:
	QOLTCommands();

	const ErrorStrings &commandErrors() const { return m_cmdErrors; }
	const ErrorStrings &loginErrors() const { return m_loginErrors; }
};

#endif // QOLTCOMMANDS_H
