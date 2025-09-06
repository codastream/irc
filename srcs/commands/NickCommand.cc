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

	void NickCommand::execute(Server& s, ClientConnection& co)
	{
		(void) s;
		(void) co;
		std::cout << "Execution" << std::endl;
	}

}