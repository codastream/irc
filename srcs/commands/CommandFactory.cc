#include "CommandFactory.h"

namespace Irc {
/************************************************************
*		🥚 CONSTRUCTORS & DESTRUCTOR						*
************************************************************/

CommandFactory::CommandFactory(void) {}

CommandFactory::~CommandFactory(void) {}

/************************************************************
*		➕ OPERATORS											*
************************************************************/

/*************************************************************
*		🛠️ FUNCTIONS											*
*************************************************************/

// TODO
ACommand* CommandFactory::create(std::string& command_name)
{
	if (command_name == "NICK")
		return new NickCommand();
	return NULL;
}

/*************************************************************
*		👁️‍ GETTERS and SETTERS				 				*
*************************************************************/

}