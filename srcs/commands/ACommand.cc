#include "commands/ACommand.h"

namespace Irc {
/************************************************************
*	🥚 CONSTRUCTORS & DESTRUCTOR							*
************************************************************/
ACommand::ACommand() : args_() {}

ACommand::ACommand(std::vector<std::string> args) : args_(args) {}

ACommand::ACommand(const ACommand& other) {
	(void) other;
}

ACommand::~ACommand(void) {}

/************************************************************
*	➕ OPERATORS												*
************************************************************/

ACommand& ACommand::operator=(const ACommand& inst) 
{
	if (this != &inst)
	{
	}
	return (*this);
}

/*************************************************************
*	🛠️ FUNCTIONS												*
*************************************************************/

/*************************************************************
*	👁️‍ GETTERS and SETTERS						 			*
*************************************************************/

}