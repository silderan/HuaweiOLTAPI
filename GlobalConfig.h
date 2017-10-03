#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QString>
#include "QIniFile.h"

class GlobalConfigData : protected QIniData
{
public:
	void setHost(const QString &host) { QIniData::insert("host", host); }
	QString host() const { return QIniData::value("host");	}

	void setUName(const QString &uname) { QIniData::insert("uname", uname); }
	QString uname() const { return QIniData::value("uname");	}

	void setUPass(const QString &upass) { QIniData::insert("upass", upass); }
	QString upass() const { return QIniData::value("upass");	}

	void setPort(quint16 port) { QIniData::insert("port", QString::number(port)); }
	quint16 port() const { return QIniData::value("port").toInt(); }
};

class GlobalConfig : public QIniFile, public GlobalConfigData
{
public:
	void save() { QIniFile::save("HuaweiOLTAPI.ini", *this); }
	void load() { QIniFile::load("HuaweiOLTAPI.ini", this); }
};

extern GlobalConfig globalConfig;

#endif // GLOBALCONFIG_H
