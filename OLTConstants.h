#ifndef QOLTCONSTANTS_H
#define QOLTCONSTANTS_H

#include <QMap>
#include <QStringList>
#include <qstring.h>

class OLTConstants
{
public:
	class ConstantsBase  : public QMap<QString, QString>
	{
	public:
		void fromFileLine(const QByteArray &line);
	};

	class Constants : public ConstantsBase
	{
	public:
		QString askForUsername() const;
		QString askForPassword() const;
	};

	class Commands : public ConstantsBase
	{
	};

	class Prompts : public ConstantsBase
	{
	public:
		QString prompt(const QString &name) const;
		QString basic() const;
		QString initial() const;
		QString config() const;
		QString admin() const;
	};

	class ErrorStrings : public ConstantsBase
	{
	public:
		QString errorString(const QString &text) const;
	};

private:
	Constants m_constants;
	Prompts m_prompts;
	Commands m_oltCommands;
	ErrorStrings m_cmdErrors;
	ErrorStrings m_loginErrors;
	void loadOLTConstantsFile();

	QString parseCommand(QString command, const QStringList data) const;
public:
	OLTConstants();

	// Prompts
	QString promptBasic() const { return m_prompts.basic(); }
	QString promptInitial() const { return m_prompts.initial(); }
	QString promptAdmin() const { return m_prompts.admin(); }
	QString promptConfig() const { return m_prompts.config(); }

	// Error strings.
	const ErrorStrings &commandErrors() const { return m_cmdErrors; }
	const ErrorStrings &loginErrors() const { return m_loginErrors; }

	// Constants.
	QString constantUName() const { return m_constants.askForUsername(); }
	QString constantUPass() const { return m_constants.askForPassword(); }

	// Commands.
	QString commandEnableAdmin() const { return m_oltCommands.value("EnableAdmin"); }
	QString commandEnterConfigMode() const { return m_oltCommands.value("EnterConfig"); }
	QString scroll(quint16 lines) const { return parseCommand(m_oltCommands.value("scroll"), QStringList() << "{lines}" << QString::number(lines)); }
	QString boardInfo(quint8 frame, quint8 slot) const { return parseCommand(m_oltCommands.value("BoardInfo"), QStringList() << "{frame}" << QString::number(frame) << "{slot}" << QString::number(slot)); }
	QString unmanaged() const { return m_oltCommands.value("GetUnmanaged"); }
};

extern OLTConstants oltConstants;

#endif // QOLTCONSTANTS_H
