#include "IRCException.h"

namespace Irc {

	/************************************************************
	*			🥚 CONSTRUCTORS & DESTRUCTOR					*
	************************************************************/

	IRCException::IRCException(ReplyCode code, const std::string& msg) : message_(msg), code_(code) {}
	IRCException::IRCException(ReplyCode code) : message_(""), code_(code) {}

	IRCException::~IRCException(void) throw() {}

	/************************************************************
	*			➕ OPERATORS										*
	************************************************************/

	/*************************************************************
	*			🛠️ FUNCTIONS										*
	*************************************************************/

	ReplyCode IRCException::getCode()
	{
		return code_;
	}

	const char* IRCException::what() const throw()
	{
		return message_.c_str();
	}

}