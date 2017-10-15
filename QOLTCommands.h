#ifndef OLTCOMMANDS_H
#define OLTCOMMANDS_H

#include <QSpinBox>
#include <QCheckBox>
#include <QEvent>

#include "QTelnetInterface.h"
#include "OLTCommands_BoardInfo.h"
#include "OLTCommands_UnmanagedOLTs.h"
#include "OLTCommands_ONTWANInfo.h"
#include "OLTCommands_ONTMACInfo.h"
#include "OLTCommands_ONTVersion.h"
#include "OLTCommands_GPONServiceProfile.h"
#include "OLTCommands_TrafficTableIP.h"
#include "OLTCommands_DBAProfile.h"
#include "OLTCommands_LineProfile.h"
#include "OLTCommands_ServicePort.h"

class QSpeedSpinBox : public QSpinBox
{
protected:
	bool event(QEvent *e);

public:
	QSpeedSpinBox(QWidget *papi = Q_NULLPTR) :
		QSpinBox(papi)
	{
		setSingleStep(64);
		setMinimum(128);
		setMaximum(1024*1024);
		setSuffix(" Kbps");
	}
};

class QTypeSpinBox : public QSpinBox
{
Q_OBJECT

	QSpinBox *m_fix;
	QSpinBox *m_assured;
	QSpinBox *m_max;
	QCheckBox *m_compensate;

public:
	QTypeSpinBox(QWidget *papi = Q_NULLPTR, QSpinBox *fix = Q_NULLPTR, QSpinBox *assured = Q_NULLPTR, QSpinBox *max = Q_NULLPTR, QCheckBox *compensate = Q_NULLPTR):
		QSpinBox(papi), m_fix(fix), m_assured(assured), m_max(max), m_compensate(compensate)
	{
		connect(this, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
	}
	void setSpeedWidgets(QSpinBox *fix, QSpinBox *assured, QSpinBox *max, QCheckBox *compensate)
	{
		m_fix = fix;
		m_assured = assured;
		m_max = max;
		m_compensate = compensate;
	}

private slots:
	void onValueChanged(int type);

protected:
	bool event(QEvent *e);
};

class QOLTCommands : public QTelnetInterface
{
Q_OBJECT

public:
	QOLTCommands();
	void setAdminMode();
	void setConfigMode();
	void quitContext();
	void setScroll(int lines);
	void getBoardInfo(quint8 frame, quint8 slot);
	void getUnmanaged();
	void getONTWANInfo(int frame, int slot, int port, int ontid);
	void getONTMACInfo(int frame, int slot, int port, int ontid);
	void getONTVersion(int frame, int slot, int port, int ontid);

	void getGPONServiceProfiles();
	void getGPONServiceProfile(int serviceProfileID);
	void enterGPONSrvcPrfl(int serviceProfileID);

	void getTrafficTableIPs();
	void getTrafficTableIP(int index);
	void addTrafficTableIP(const QString &name, int cir, int pir, int priority);
	void modTrafficTableIP(int index, const QString &name, int cir, int pir, int priority);
	void delTrafficTableIP(int index);

	void getDBAProfiles();
	void getDBAProfile(int index);
	void addDBAProfile(const QString &name, int type, int fix, int assured, int max, bool compensate);
	void modDBAProfile(int index, const QString &name, int type, int fix, int assured, int max, bool compensate);
	void delDBAProfile(int id);

	void getServicePorts();
	void getServicePort(int index);
	void getONTServicePort(int frame, int slot, int port, int ontid);
	void addServicePort(int frame, int slot, int port, int ontid, int oltVLAN, int ontVLAN, int gemport, int upTrafficIndex = -1, int dwTrafficIndex = -1);
//	void modServicePort(int frame, int slot, int port, int ontid, int oltVLAN, int ontVLAN, int gemport, int upTrafficIndex = -1, int dwTrafficIndex = -1);
	void delServicePort(int index);

	void getGPONLineProfiles();
	void getGPONLineProfile(int id);
	void addGPONLineProfile(const QString &name);
//	void modLineProfile(const QString &name, const QString &type, const QString &speeds);
	void delGPONLineProfile(int id);

private slots:
	void onCommandReceived(const QString &tag, const QString &cmd, const QString data);
	void onDisconnected();

signals:
	void boardInfo(const OLTCommands::BoardInfo &);
	void unmanagedOnts(const OLTCommands::UnmanagedONTs &);
	void ontWANInfo(const OLTCommands::ONTWANInfo &);
	void ontMACInfo(const OLTCommands::ONTMACInfo &);
	void ontVersionInfo(const OLTCommands::ONTVersion &);

	void gponServiceProfiles(const OLTCommands::GPONServiceProfiles &);
	void gponServiceProfile(const OLTCommands::GPONServiceProfile &);

	void trafficTableIPs(const OLTCommands::TrafficTableIPs &);
	void trafficTableIP(const OLTCommands::TrafficTableIP &);

	void dbaProfiles(const OLTCommands::DBAProfiles &);
	void dbaProfile(const OLTCommands::DBAProfile &);

	void servicePorts(const OLTCommands::ServicePorts &);
	void servicePort(const OLTCommands::ServicePort &);

	void lineProfiles(const OLTCommands::LineProfiles &);
	void lineProfile(const OLTCommands::LineProfile &);
};

#endif // OLTCOMMANDS_H
