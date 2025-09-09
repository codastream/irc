#include "ReplyAssert.h"

/************************************************************
*		🥚 CONSTRUCTORS & DESTRUCTOR						*
************************************************************/

ReplyAssert::ReplyAssert(const std::string& reply) : reply_(reply), args_()
{
	std::stringstream ss(reply_);
	if (reply_[0] == ':')
		ss >> prefix_;
	ss >> cmd_or_code_;
	std::string token;
	while (ss >> token)
	{
		if (token[0] == ':')
		{
			trailing_ = token;
			break ;
		}
		args_.push_back(token);
	}
}

ReplyAssert::~ReplyAssert(void) {}

/************************************************************
*		➕ OPERATORS											*
************************************************************/

/*************************************************************
*		🛠️ FUNCTIONS											*
*************************************************************/

ReplyAssert&	ReplyAssert::hasCode(Irc::ReplyCode code) 
{
	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(3) << code;

	if (cmd_or_code_ != oss.str())
	{
		throw std::runtime_error("Expected code " + oss.str() + " not found ");
	}
	return *this;
}

ReplyAssert&	ReplyAssert::endsWith(const std::string& trailing) 
{
	if (trailing_ != trailing)
	{
		throw std::runtime_error("Expected trailing message " + trailing + " not found ");
	}
	return *this;
}

ReplyAssert&	ReplyAssert::contains(const std::string& token) 
{
	if (!(reply_.find(token) == std::string::npos))
	{
		throw std::runtime_error("Expected token " + token + " not found ");
	}
	return *this;
}

/*************************************************************
*		👁️‍ GETTERS and SETTERS				 				*
*************************************************************/
