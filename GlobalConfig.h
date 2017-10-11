#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QString>
#include "QIniFile.h"

class GlobalConfigData : protected QIniData
{
public:
	void setHostName(const QString &host) { QIniData::insert("hostName", host); }
	QString hostName() const { return QIniData::value("hostName");	}

	void setHostUName(const QString &uname) { QIniData::insert("oltUName", uname); }
	QString oltUName() const { return QIniData::value("oltUName");	}

	void sethostUPass(const QString &upass) { QIniData::insert("oltUPass", upass); }
	QString oltUPpass() const { return QIniData::value("oltUPass");	}

	void setHostPort(quint16 port) { QIniData::insert("hostPort", QString::number(port)); }
	quint16 hostPort() const { return QIniData::value("hostPort").toInt(); }

	void setCommand(const QString &cmd ) { QIniData::insert("command", cmd); }
	QString command() const { return QIniData::value("command"); }

	void setSerialNumber(const QString &cmd ) { QIniData::insert("serial", cmd); }
	QString serialNumber() const { return QIniData::value("serial"); }

	void setValueNumber(int v) { QIniData::insert("valueNumber", QString::number(v)); }
	int valueNumber() const { return QIniData::value("valueNumber").toInt(); }

	void setValueName(const QString &v) { QIniData::insert("valueName", v); }
	QString valueName() const { return QIniData::value("valueName"); }

	void setFrameNumber(int v) { QIniData::insert("frameNumber", QString::number(v)); }
	int frameNumber() const { return QIniData::value("frameNumber").toInt(); }

	void setSlotNumber(int v) { QIniData::insert("slotNumber", QString::number(v)); }
	int slotNumber() const { return QIniData::value("slotNumber").toInt(); }

	void setPortNumber(int v) { QIniData::insert("portNumber", QString::number(v)); }
	int portNumber() const { return QIniData::value("portNumber").toInt(); }

	void setCIRNumber(int v) { QIniData::insert("cirNumber", QString::number(v)); }
	int cirNumber() const { return QIniData::value("cirNumber").toInt(); }

	void setPIRNumber(int v) { QIniData::insert("pirNumber", QString::number(v)); }
	int pirNumber() const { return QIniData::value("pirNumber").toInt(); }

	void setPriorityNumber(int v) { QIniData::insert("priorityNumber", QString::number(v)); }
	int priorityNumber() const { return QIniData::value("priorityNumber").toInt(); }

	void setCustomCmd(const QString &cmd) { QIniData::insert("customCmd", cmd); }
	QString customCmd() const { return QIniData::value("customCmd"); }
};

class GlobalConfig : public QIniFile, public GlobalConfigData
{
public:
	void save() { QIniFile::save("HuaweiOLTAPI.ini", *this); }
	void load() { QIniFile::load("HuaweiOLTAPI.ini", this); }
};

extern GlobalConfig globalConfig;

#endif // GLOBALCONFIG_H
