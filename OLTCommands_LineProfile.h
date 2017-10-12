#ifndef OLTCOMMANDS_LINEPROFILE_H
#define OLTCOMMANDS_LINEPROFILE_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands
{
struct LineProfileInfoBase
{

};

struct LineProfileInfo : LineProfileInfoBase
{

};

class LineProfileInfoList : public QList<LineProfileInfoBase>
{

};

class LineProfile : public OLTCommandReply
{
	LineProfileInfo m_lineProfile;

public:
	LineProfile(const QString &label,const QString &command, const QString &rawData);
};

class LineProfiles : public OLTCommandReply
{
	LineProfileInfoList m_lineProfiles;

public:
	LineProfiles(const QString &label,const QString &command, const QString &rawData);
};
}
#endif // OLTCOMMANDS_LINEPROFILE_H
