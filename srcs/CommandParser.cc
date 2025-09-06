#include "CommandParser.h"

namespace Irc {

	/************************************************************
	*		🥚 CONSTRUCTORS & DESTRUCTOR						*
	************************************************************/

	CommandParser::CommandParser(void) {}

	CommandParser::~CommandParser(void) {}

	/************************************************************
	*		➕ OPERATORS											*
	************************************************************/

	/*************************************************************
	*		🛠️ FUNCTIONS											*
	*************************************************************/

	/// @brief checks that last 2 characters are ASCII 13 and 10
	/// @param msg 
	/// @return 
	bool	check_format(const std::string& msg)
	{
		std::string::const_reverse_iterator it = msg.rbegin();
		if (*it != '\n')
			return false;
		++it;
		if (*it != '\r')
			return false;
		return true;
	}

	ACommand*	CommandParser::parseCommand(const std::string& s)
	{
		Message 	msg;
		std::string	token;
		ACommand*	cmd;

		if (!check_format(s))
			throw IRCException(FORMAT_ERR, "should end with \\r\\n");
		std::istringstream iss(s);
		if (!(iss >> msg.command_name))
			throw IRCException(FORMAT_ERR, "should start with command name");
		while (iss >> token)
		{
			msg.params.push_back(token);
		}
		cmd = CommandFactory::create(msg.command_name, msg.params);
		return cmd;
	}

	/*************************************************************
	*		👁️‍ GETTERS and SETTERS				 				*
	*************************************************************/

}