#ifndef OLTCOMMANDS_H
#define OLTCOMMANDS_H

#include "QTelnetInterface.h"

class CommandReceivedInfo
{
	QString m_tag;
	QString m_command;
	QString m_raw;

public:
	CommandReceivedInfo(const QString &tag,const QString &command, const QString &rawData) :
		m_tag(tag), m_command(command), m_raw(rawData)
	{
	}

protected:
	virtual QStringList toStringInfoData(bool includeRaw = false) const
	{
		QStringList rtn = QStringList()
				<< QObject::tr("TAG") << m_tag
				<< QObject::tr("Command") << m_command;
		if( includeRaw )
		{
			rtn.append("Rar data");
			rtn.append(m_raw);
		}
		return rtn;
	}

	struct OntBasicInfo
	{
		quint8 frame;
		quint8 slot;
		quint8 port;
		quint8 id;
		QString serial;
		QStringList fromString(const QString &txt, int framePos, int slotPos, int portPos, int idPos, int serialPos);
	};

public:
	QString toString() const
	{
		QStringList info = toStringInfoData(true);
		QString rtn;
		for( int i = 0; i < info.count(); i+= 2 )
			rtn += QString("%1: %2\n").arg(info[i+0], info[i+1]);
		return rtn;
	}
};

class BoardInfo : public CommandReceivedInfo
{
public:
	struct PortInfo
	{
		QString portType;
		quint8 minDistance;
		quint8 maxDistance;
		QString opticalStatus;
	};
	typedef QList<PortInfo> PortInfoList;

	struct OntInfo : public OntBasicInfo
	{
		QString controlFlag;
		QString runState;
		QString configState;
		QString matchState;
		QString protectSide;
		QString descript;
	};
	typedef QList<OntInfo> OntInfoList;

private:
	QString m_boardName;
	QString m_boardStatus;
	quint8 frame;
	PortInfoList m_portInfoList;
	OntInfoList m_ontInfoList;

public:
	BoardInfo(const QString &tag,const QString &command, const QString &rawData);

protected:
	virtual QStringList toStringInfoData(bool includeRaw) const;
};

#ifndef QT_NO_DEBUG
QString offlineResponse(const QString &id);
#endif

class OLTCommands : public QTelnetInterface
{
Q_OBJECT

public:
	OLTCommands();
	void enableAdminMode();
	void enterConfigMode();
	void scroll(int lines);
	void boardInfo(quint8 frame, quint8 slot);

private slots:
	void onCommandReceived(const QString &tag, const QString &cmd, const QString data);

signals:
	void boardInfoReceived(const BoardInfo &boardInfo);
};

#endif // OLTCOMMANDS_H
