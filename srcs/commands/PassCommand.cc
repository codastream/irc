#include "PassCommand.h"

namespace Irc {
/************************************************************
*		🥚 CONSTRUCTORS & DESTRUCTOR						*
************************************************************/

PassCommand::PassCommand(void) : ACommand() {}
PassCommand::PassCommand(std::vector<std::string> args) : ACommand(args) {}

PassCommand::~PassCommand(void) {}

/************************************************************
*		➕ OPERATORS											*
************************************************************/

/*************************************************************
*		🛠️ FUNCTIONS											*
*************************************************************/

void PassCommand::execute(Server& s, ClientConnection& co)
{
	if (args_.size() < 1)
	{
		co.queue_err_needmoreparams("PASS");
		return ;
	}
	std::string&	password = args_[0];
	Client* 		c = s.get_client_by_fd(co.get_fd());
	if (!c)
	{
		Logger::error(std::string("client not found for fd ") + Utils::str(co.get_fd()));
		throw std::runtime_error("client not found for fd");
	}
	if (c->get_status() == REGISTERED)
	{
		co.queue_err_alreadyregistered(c->get_nick());
	}
	if (!s.is_valid_password(password))
	{
		co.queue_err_passwdmismatch();
		return ;
	}
	c->set_status(AUTHENTICATED);
}

/*************************************************************
*		👁️‍ GETTERS and SETTERS				 				*
*************************************************************/

}