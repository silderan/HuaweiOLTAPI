#ifndef OLTCOMMANDS_DBAPROFILE_H
#define OLTCOMMANDS_DBAPROFILE_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands
{
struct DBAProfileInfoBase
{

};

struct DBAProfileInfo : DBAProfileInfoBase
{

};

class DBAProfileInfoList : public QList<DBAProfileInfoBase>
{

};

class DBAProfile : public OLTCommandReply
{
	DBAProfileInfo m_dbaProfile;

public:
	DBAProfile(const QString &label,const QString &command, const QString &rawData);
};

class DBAProfiles : public OLTCommandReply
{
	DBAProfileInfoList m_dbaProfiles;

public:
	DBAProfiles(const QString &label,const QString &command, const QString &rawData);
};
} // End namespace
#endif // OLTCOMMANDS_DBAPROFILE_H
