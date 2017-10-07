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
	void clear()
	{
		m_tag.clear();
		m_command.clear();
		m_raw.clear();
	}

	static QStringList splitLines(const QString &txt);
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

class BoardInfo : public CommandReceivedInfo
{
public:
	struct PortInfo
	{
		QString portType;
		quint8 minDistance;
		quint8 maxDistance;
		QString opticalStatus;
		void clear()
		{
			portType.clear();
			minDistance = 0;
			maxDistance = 0;
			opticalStatus.clear();
		}
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
		void clear()
		{
			OntBasicInfo::clear();
			controlFlag.clear();
			runState.clear();
			configState.clear();
			matchState.clear();
			protectSide.clear();
			descript.clear();
		}
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

	void clear()
	{
		CommandReceivedInfo::clear();
		m_boardName.clear();
		m_boardStatus.clear();
		frame = 0;
		m_portInfoList.clear();
		m_ontInfoList.clear();
	}

protected:
	virtual QStringList toStringInfoData(bool includeRaw) const;
};

class UnmanagedONTs : public CommandReceivedInfo
{
protected:
	struct OntInfo : public OntBasicInfo
	{
		QString password;
		QString loid;
		QString checkcode;
		QString vendorID;
		QString hwVersion;
		QString swVersion;
		QString equipID;
		void clear()
		{
			OntBasicInfo::clear();
			password.clear();
			loid.clear();
			checkcode.clear();
			vendorID.clear();
			hwVersion.clear();
			swVersion.clear();
			equipID.clear();
		}
		QStringList toStringInfoData() const
		{
			return OntBasicInfo::toStringInfoData() + QStringList()
					<< "VendorID" << vendorID
					<< "EquipID" << equipID
					<< "hardware" << hwVersion
					<< "Software" << swVersion
					<< "LOID" << loid
					<< "Checkcode" << checkcode;
		}
	};
	typedef QList<OntInfo> OntInfoList;

private:
	OntInfoList m_unmanagedOnts;

public:
	UnmanagedONTs(const QString &tag,const QString &command, const QString &rawData);

	void clear()
	{
		CommandReceivedInfo::clear();
		m_unmanagedOnts.clear();
	}

protected:
	virtual QStringList toStringInfoData(bool includeRaw) const;
};

class OLTCommands : public QTelnetInterface
{
Q_OBJECT

public:
	OLTCommands();
	void setAdminMode();
	void setConfigMode();
	void setScroll(int lines);
	void getBoardInfo(quint8 frame, quint8 slot);
	void getUnmanaged();

private slots:
	void onCommandReceived(const QString &tag, const QString &cmd, const QString data);

signals:
	void boardInfo(const BoardInfo &getBoardInfo);
	void unmanagedOnts(const UnmanagedONTs &unmanaged);
};

#endif // OLTCOMMANDS_H
