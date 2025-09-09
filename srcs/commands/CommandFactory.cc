#include "commands/CommandFactory.h"

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
	ACommand* CommandFactory::create(std::string& command_name, std::vector<std::string> params)
	{
		ACommand* cmd = NULL;
		if (command_name == "PASS")
			cmd = new PassCommand(params);
		else if (command_name == "NICK")
			cmd = new NickCommand(params);
		return cmd;
	}


}