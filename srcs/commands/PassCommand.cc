#include "PassCommand.h"

namespace Irc {
/************************************************************
*		🥚 CONSTRUCTORS & DESTRUCTOR						*
************************************************************/

PassCommand::PassCommand(void) : ACommand() {}
PassCommand::PassCommand(std::vector<std::string> args) : ACommand(args) 
{
	std::cout << "new Path command with args" << std::endl;
}

PassCommand::~PassCommand(void) {}

/************************************************************
*		➕ OPERATORS											*
************************************************************/

/*************************************************************
*		🛠️ FUNCTIONS											*
*************************************************************/

void PassCommand::execute(Server& s, ClientConnection& co)
{
	ReplyFactory& rf = s.get_reply_factory();

	if (args_.size() < 1)
	{
		co.queue_reply(rf.make_reply(ERR_NEEDMOREPARAMS, "PASS", "Not enough parameters"));
		return ;
	}
	if (args_.size() > 1)
	{
		throw IRCException(FORMAT_ERR);
	}
	Client c = s.get_client_by_fd(co.get_fd());
	if (c.get_status() == REGISTERED)
	{
		co.queue_reply(rf.make_reply(ERR_ALREADYREGISTERED, c.get_nick(), "Unauthorized command (already registered)"));
		return ;
	}
	if (!s.is_valid_password(args_[0]))
	{
		co.queue_reply(rf.make_reply(ERR_PASSWDMISMATCH, "*", "Password incorrect"));
		return ;
	}
	c.set_status(AUTHENTICATED);
	std::cout << "end of execute\n";
}

/*************************************************************
*		👁️‍ GETTERS and SETTERS				 				*
*************************************************************/

}