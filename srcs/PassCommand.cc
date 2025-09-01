#include "PassCommand.h"

namespace Irc {
/************************************************************
*		🥚 CONSTRUCTORS & DESTRUCTOR						*
************************************************************/

PassCommand::PassCommand(void) {}

PassCommand::~PassCommand(void) {}

/************************************************************
*		➕ OPERATORS											*
************************************************************/

PassCommand& PassCommand::operator=(const PassCommand& inst) 
{
	if (this != &inst)
	{
	}
	return (*this);
}

/*************************************************************
*		🛠️ FUNCTIONS											*
*************************************************************/

void PassCommand::execute(Server& s, Client& c, Message& m)
{
	(void) s;
	(void) c;
	(void) m;
	std::cout << "Execution" << std::endl;
}

/*************************************************************
*		👁️‍ GETTERS and SETTERS				 				*
*************************************************************/

}