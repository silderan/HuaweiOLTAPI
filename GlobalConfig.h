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

	void setONTID(int v) { QIniData::insert("ontID", QString::number(v)); }
	int ontID() const { return QIniData::value("ontID").toInt(); }

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

	void setONTVLAN(int v) { QIniData::insert("ontVLAN", QString::number(v)); }
	int ontVLAN() const { return QIniData::value("ontVLAN").toInt(); }

	void setOLTVLAN(int v) { QIniData::insert("oltVLAN", QString::number(v)); }
	int oltVLAN() const { return QIniData::value("oltVLAN").toInt(); }

	void setGEMPort(int v) { QIniData::insert("GEMPort", QString::number(v)); }
	int gemPort() const { return QIniData::value("GEMPort").toInt(); }

	void setDwTrafficTable(int v) { QIniData::insert("DwTrafficTable", QString::number(v)); }
	int dwTrafficTable() const { return QIniData::value("DwTrafficTable").toInt(); }

	void setUpTrafficTable(int v) { QIniData::insert("UpTrafficTable", QString::number(v)); }
	int upTrafficTable() const { return QIniData::value("UpTrafficTable").toInt(); }

	void setType(int t) { QIniData::insert("type", QString::number(t)); }
	int type() const { return QIniData::value("type").toInt(); }

	void setFix(int f) { QIniData::insert("fix", QString::number(f)); }
	int fix() const { return QIniData::value("fix").toInt(); }

	void setAssured(int a) { QIniData::insert("assured", QString::number(a)); }
	int assured() const { return QIniData::value("assured").toInt(); }

	void setMax(int m) { QIniData::insert("max", QString::number(m)); }
	int max() const { return QIniData::value("max").toInt(); }
};

class GlobalConfig : public QIniFile, public GlobalConfigData
{
public:
	void save() { QIniFile::save("HuaweiOLTAPI.ini", *this); }
	void load() { QIniFile::load("HuaweiOLTAPI.ini", this); }
};

extern GlobalConfig globalConfig;

#endif // GLOBALCONFIG_H
