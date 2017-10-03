#ifndef QHUAWEICOMMANDS_H
#define QHUAWEICOMMANDS_H

#include "QTelnetInterface.h"

class QHuaweiCommands : public QTelnetInterface
{
	ErrorStrings m_comandErrors;

public:
	QHuaweiCommands();
	void enableAdminMode();
	void enterConfigMode();
};

#endif // QHUAWEICOMMANDS_H
