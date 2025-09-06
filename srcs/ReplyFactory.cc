#include "ReplyFactory.h"

namespace Irc {

	/************************************************************
	*		🥚 CONSTRUCTORS & DESTRUCTOR						*
	************************************************************/

	ReplyFactory::ReplyFactory(const std::string& server_name) : server_name_(server_name) {}

	ReplyFactory::~ReplyFactory(void) {}

	/************************************************************
	*		➕ OPERATORS											*
	************************************************************/

	/*************************************************************
	*		🛠️ FUNCTIONS											*
	*************************************************************/

	std::string	format_code(ReplyCode code)
	{
		std::ostringstream oss;
		oss << std::setfill('0') << std::setw(3) << static_cast<int>(code);
		return oss.str();
	}

	std::string	ReplyFactory::make_reply(ReplyCode code, const std::string& target, \
		const std::string& trailing_msg = "")
	{
		std::string reply = ":" + server_name_ + " " + format_code(code) + " " + target;
		if (!trailing_msg.empty())
			reply += " :" + trailing_msg;
		return reply + "\r\n";
	}

	std::string ReplyFactory::make_reply(ReplyCode code, const std::string& target, \
		std::vector<std::string>& params, const std::string& trailing_msg = "")
	{
		std::string reply = ":" + server_name_ + " " + format_code(code) + " " + target;
		if (!params.empty())
		{
			for (std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); ++it)
				reply += " " + *it;
		}
		if (!trailing_msg.empty())
			reply += " :" + trailing_msg;
		return reply + "\r\n";
	}
}