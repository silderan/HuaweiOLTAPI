#ifndef TRAFFICTABLEIP_H
#define TRAFFICTABLEIP_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands
{
struct TrafficTableBasicIPInfo
{
	int id;
	int cir;
	int cbs;
	int pir;
	int pbs;
	int priority;
	QString copyPolicy;
	QString priorityPolicy;
	TrafficTableBasicIPInfo()
	{
		clear();
	}

	void clear()
	{
		id = 0;
		cir = 0;
		cbs = 0;
		pir = 0;
		pbs = 0;
		priority = 0;
		copyPolicy.clear();
		priorityPolicy.clear();
	}
	virtual QStringList toStringInfoData() const
	{
		return QStringList()
				<< QObject::tr("Table index") << QString::number(id)
				<< QObject::tr("CIR") << QString("%1 Kbps").arg(cir)
				<< QObject::tr("CBS") << QString("%1 bytes").arg(cbs)
				<< QObject::tr("PIR") << QString("%1 Kbps").arg(pir)
				<< QObject::tr("PBS") << QString("%1 bytes").arg(pbs)
				<< QObject::tr("Traffic priority") << QString::number(priority)
				<< QObject::tr("Copy policy") << copyPolicy
				<< QObject::tr("Priority policy") << priorityPolicy;
	}
	virtual ~TrafficTableBasicIPInfo()
	{	}
};

struct TrafficTableIPInfo : public TrafficTableBasicIPInfo
{
	QString name;
	QString mappingIndex;
	QString ctagMappingPriority;
	QString ctagMappingIndex;//   : -
	QString ctagDefaultPriority;//: 0
	QString priorityPolicy;//      : local-pri
	int fix;//                  : 0 kbps
	QString carThresholdProfile;//: -
	QString colorMode;//           : color-blind
	QString colorPolicy;//         : dei
	QString referencedStatus;//    : used
	TrafficTableIPInfo() : TrafficTableBasicIPInfo()
	{
		clear();
	}

	void clear()
	{
		TrafficTableBasicIPInfo::clear();
		name.clear();
		mappingIndex.clear();
		ctagMappingPriority.clear();
		ctagMappingIndex.clear();
		ctagDefaultPriority.clear();
		priorityPolicy.clear();
		fix = 0;
		carThresholdProfile.clear();
		colorMode.clear();
		colorPolicy.clear();
		referencedStatus.clear();
	}
	virtual QStringList toStringInfoData() const
	{
		return TrafficTableBasicIPInfo::toStringInfoData()
				<< QObject::tr("Nombre") << name
				<< QObject::tr("Mapping index") << mappingIndex
				<< QObject::tr("CTAG Mapping priority") << ctagMappingPriority
				<< QObject::tr("CTAG Mapping Index") << ctagMappingIndex
				<< QObject::tr("CTAG Default Priority") << ctagDefaultPriority
				<< QObject::tr("Priority policy") << priorityPolicy
				<< QObject::tr("Fix") << QString("%1 Kbps").arg(fix)
				<< QObject::tr("CAR Threshold Profile") << carThresholdProfile
				<< QObject::tr("Color mode") << colorMode
				<< QObject::tr("Color policy") << colorPolicy
				<< QObject::tr("Referenced?") << referencedStatus;

	}
};

class TrafficTableIPList : public QList<TrafficTableBasicIPInfo>
{
public:
	virtual QStringList toStringInfoData() const
	{
		QStringList rtn;
		for( int i = 0; i < count(); i++ )
		{
			rtn << "" << QObject::tr("Traffic table number %1").arg(i)
				<< at(i).toStringInfoData();
		}
		return rtn;
	}
};

class TrafficTableIP : public OLTCommandReply
{
	TrafficTableIPInfo m_trafficTableIPInfo;

public:
	TrafficTableIP(const QString &label,const QString &command, const QString &rawData);
	virtual QStringList toStringInfoData() const
	{
		return m_trafficTableIPInfo.toStringInfoData();
	}
};

class TrafficTableIPs : public OLTCommandReply
{
	TrafficTableIPList m_trafficTableIPList;
protected:
	virtual QStringList toStringInfoData() const
	{
		return m_trafficTableIPList.toStringInfoData();
	}
public:
	TrafficTableIPs(const QString &label,const QString &command, const QString &rawData);
};
}
#endif // TRAFFICTABLEIP_H
