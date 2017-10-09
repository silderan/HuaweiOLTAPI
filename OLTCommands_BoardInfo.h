#ifndef OLTCOMMAND_BOARDINFO_H
#define OLTCOMMAND_BOARDINFO_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands { class BoardInfo; }

class BoardInfo : public OLTCommandReply
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
		OLTCommandReply::clear();
		m_boardName.clear();
		m_boardStatus.clear();
		frame = 0;
		m_portInfoList.clear();
		m_ontInfoList.clear();
	}

protected:
	virtual QStringList toStringInfoData(bool includeRaw) const;
};

#endif // OLTCOMMAND_BOARDINFO_H
