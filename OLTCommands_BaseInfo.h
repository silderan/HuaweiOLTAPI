#ifndef OLTCOMMANDS_BASEINFO_H
#define OLTCOMMANDS_BASEINFO_H

#include <QStringList>

namespace OLTCommands { class OLTCommandReply; }

class OLTCommandReply
{
	QString m_tag;
	QString m_command;
	QString m_raw;

public:
	OLTCommandReply(const QString &tag,const QString &command, const QString &rawData) :
		m_tag(tag), m_command(command), m_raw(rawData)
	{
	}
	virtual ~OLTCommandReply()
	{	}
	void clear()
	{
		m_tag.clear();
		m_command.clear();
		m_raw.clear();
	}

	static QStringList splitLines(const QString &txt);
	static int splitField(const QString &line, QString &key, QString &value);
	static QString centeredText( const QString &fillText, const QString &centerText, int lineLenght);
	QStringList toStringInfoDataBase() const
	{
		return QStringList()
				<< QObject::tr("TAG") << m_tag
				<< QObject::tr("Command") << m_command;
	}
protected:
	virtual QStringList toStringInfoData() const
	{	return QStringList(); }

	struct OntBasicInfo
	{
		quint8 frame;
		quint8 slot;
		quint8 port;
		quint8 id;
		QString serial;
		QStringList fromString(const QString &txt, int framePos, int slotPos, int portPos, int idPos, int serialPos);
		QStringList toStringInfoData() const
		{
			return QStringList() << "ONT Basic Info" << QString("%1/%2/%3-%4 %5").arg(frame).arg(slot).arg(port).arg(id).arg(serial);
		}
		void clear()
		{
			frame = 0;
			slot = 0;
			id = 0;
			port = 0;
			serial.clear();
		}
	};

public:
	QString toString() const;
};

#endif // OLTCOMMANDS_BASEINFO_H
