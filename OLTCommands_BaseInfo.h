#ifndef OLTCOMMANDS_BASEINFO_H
#define OLTCOMMANDS_BASEINFO_H

#include <QStringList>

namespace OLTCommands
{
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

class PortType
{
	bool m_isGPON;
public:
	bool isGPON()const {return m_isGPON; }
	bool isEPON()const {return !m_isGPON; }
	void setGPON(bool gpon=true) { m_isGPON = gpon; }
	void fromString(const QString &s)
		{ setGPON(s.contains("gpon", Qt::CaseInsensitive)); }
	QString toString()const { return isGPON() ? "GPON" : "EPON"; }

	void clear()
	{ m_isGPON = true; }

	PortType() : m_isGPON(true)
	{	}
	PortType(const PortType &p) : m_isGPON(p.m_isGPON)
	{	}
	PortType(const QString &s)
	{
		fromString(s);
	}
};

class OLTCommandReply
{
	QString m_tag;
	QString m_command;
	QString m_raw;

public:
	OLTCommandReply(const QString &label,const QString &command, const QString &rawData) :
		m_tag(label), m_command(command), m_raw(rawData)
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
	static int splitField(const QString &line, QString &key, QString &value, QString splitChar = QString(":"));
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

public:
	QString toString() const;
};
} // End namespace
#endif // OLTCOMMANDS_BASEINFO_H
