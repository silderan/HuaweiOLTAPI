#ifndef OLTCOMMANDS_BASEINFO_H
#define OLTCOMMANDS_BASEINFO_H

#include <QStringList>

namespace OLTCommands
{
class OLTIntValue
{
	int m_value;
	enum State
	{
		Unknown,
		Undefined,
		Defined
	}m_state;

public:
	OLTIntValue() : m_value(0), m_state(Undefined)
	{	}
	OLTIntValue(const OLTIntValue &v) : m_value(v.m_value), m_state(v.m_state)
	{	}
	OLTIntValue(const QString &v)
	{
		fromString(v);
	}
	void fromString(const QString &v)
	{
		if( v == "-" )
			m_value = Undefined;
		else
		{
			bool converted;
			m_value = v.toInt(&converted);
			m_state = converted ? Defined : Unknown;
		}
	}
	QString toString()const
	{
		switch( m_state )
		{
		case Unknown:	return "?";
		case Undefined:	return "-";
		case Defined:
			break;
		}
		return QString::number(m_value);
	}
	void clear()
	{
		m_state = Undefined;
	}
};

struct OLTStringValue
{
	QString m_value;
	enum State
	{
		Unknown,
		Undefined,
		Defined
	}m_state;

public:
	OLTStringValue() : m_state(Undefined)
	{	}
	OLTStringValue(const OLTStringValue &v) : m_value(v.m_value), m_state(v.m_state)
	{	}
	OLTStringValue(const QString &v)
	{
		fromString(v);
	}
	void fromString(const QString &v)
	{
		if( v == "-" )
			m_value = Undefined;
		else
		{
			m_value = v;
			m_state = (v == "?") ? Unknown : Defined;
		}
	}
	QString toString()const
	{
		switch( m_state )
		{
		case Unknown:	return "?";
		case Undefined:	return "-";
		case Defined:
			break;
		}
		return m_value;
	}
	void clear()
	{
		m_state = Undefined;
	}
};

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

class Priority
{
	enum
	{
		Undefined = -2,
		Unknown = -1,
		P0 = 0,
		P1,
		P2,
		P3,
		P4,
		P5,
		P6,
		P7
	}m_value;
public:
	Priority() : m_value(Undefined)
	{	}
	Priority(const QString &s)
	{	fromString(s);	}
	Priority(const Priority &p) : m_value(p.m_value)
	{	}
	void fromString(const QString &p)
	{
		if( p == "-" )			m_value = Undefined;
		else if( p == "0" )		m_value = P0;
		else if( p == "1" )		m_value = P1;
		else if( p == "2" )		m_value = P2;
		else if( p == "3" )		m_value = P3;
		else if( p == "4" )		m_value = P4;
		else if( p == "5" )		m_value = P5;
		else if( p == "6" )		m_value = P6;
		else if( p == "7" )		m_value = P7;
		else
			m_value = Unknown;
	}
	QString toString() const
	{
		switch( m_value )
		{
		case Undefined:	return "-";
		case Unknown:	return "?";
		case P0:		return "0";
		case P1:		return "1";
		case P2:		return "2";
		case P3:		return "3";
		case P4:		return "4";
		case P5:		return "5";
		case P6:		return "6";
		case P7:		return "7";
		}
		return "!?";
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
