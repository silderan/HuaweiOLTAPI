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

	void setFrameNumber(int v) { QIniData::insert("frameNumber", QString::number(v)); }
	int frameNumber() const { return QIniData::value("frameNumber").toInt(); }

	void setSlotNumber(int v) { QIniData::insert("slotNumber", QString::number(v)); }
	int slotNumber() const { return QIniData::value("slotNumber").toInt(); }

	void setPortNumber(int v) { QIniData::insert("portNumber", QString::number(v)); }
	int portNumber() const { return QIniData::value("portNumber").toInt(); }

	void setONTIDNumber(int v) { QIniData::insert("ontIDNumber", QString::number(v)); }
	int ontIDNumber() const { return QIniData::value("ontIDNumber").toInt(); }
};

class GlobalConfig : public QIniFile, public GlobalConfigData
{
public:
	void save() { QIniFile::save("HuaweiOLTAPI.ini", *this); }
	void load() { QIniFile::load("HuaweiOLTAPI.ini", this); }
};

extern GlobalConfig globalConfig;

#endif // GLOBALCONFIG_H
