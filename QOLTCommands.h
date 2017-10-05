#ifndef OLTCOMMANDS_H
#define OLTCOMMANDS_H

#include "QTelnetInterface.h"

class OLTCommands : public QTelnetInterface
{
public:
	OLTCommands();
	void enableAdminMode();
	void enterConfigMode();
	void scroll(int lines);
};

#endif // OLTCOMMANDS_H
