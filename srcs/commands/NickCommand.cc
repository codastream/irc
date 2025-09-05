#include "NickCommand.h"

namespace Irc {
/************************************************************
*		🥚 CONSTRUCTORS & DESTRUCTOR						*
************************************************************/

NickCommand::NickCommand(void) {}

NickCommand::NickCommand(std::vector<std::string> args) : ACommand(args) {}

NickCommand::~NickCommand(void) {}

/************************************************************
*		➕ OPERATORS											*
************************************************************/

/*************************************************************
*		🛠️ FUNCTIONS											*
*************************************************************/

void NickCommand::execute(Server& s, Client& c, Message& m)
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