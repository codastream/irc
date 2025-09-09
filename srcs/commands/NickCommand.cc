#include "commands/NickCommand.h"

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

	static bool is_valid_nickstring(const std::string& new_nick)
	{
		const std::string& authorized_specials = "-[]]\\^{}";

		if (!(new_nick.length() > 9 || new_nick.length() < 3))
			return false;
		if (!std::isalpha(new_nick[0]))
			return false;
		for (std::string::const_iterator it = new_nick.begin(); it != new_nick.end(); ++it)
		{
			char c = *it;
			if (!std::isalpha(c) && !std::isdigit(c) \
				&& authorized_specials.find(c) == std::string::npos)
				return false;
		}
		return true;
	}

	static bool	is_available_nick(Server& s, int client_fd, const std::string& new_nick)
	{
		Client* c = s.get_client_by_nick(new_nick);
		if (c && c->get_fd() != client_fd)
		{
			return false;
		}
		return true;
	}

	/// @brief checks nickname correctness and availability
	/// does not generate a reply if successful (not necessary as per RFC)
	/// @param s 
	/// @param co 
	void NickCommand::execute(Server& s, ClientConnection& co)
	{
		if (args_.size() < 1)
		{
			co.queue_err_nonicknamegiven();
			return ;
		}
		std::string new_nick = args_[0];
		if (!is_valid_nickstring(new_nick))
		{
			co.queue_err_erroneusnickname(args_[0]);
			return ;
		}
		if (!is_available_nick(s, co.get_fd(), new_nick))
		{
			co.queue_err_nicknameinuse(new_nick);
			return ;
		}
		Client* c = s.get_client_by_fd(co.get_fd());
		c->set_nick(new_nick);
		s.update_client_by_nick(c);
	}

}