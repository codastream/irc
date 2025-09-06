#ifndef IRCEXCEPTION_H
#define IRCEXCEPTION_H

#include "reply_codes.h"

#include <string>
#include <exception>

namespace Irc {
	class IRCException : public std::exception {
		public:

			IRCException(ReplyCode code, const std::string& msg);
			IRCException(ReplyCode code);
			virtual ~IRCException() throw();
			
			virtual const char* what() const throw();
			ReplyCode getCode();
			
		private:

			std::string	message_;
			ReplyCode code_;
	};
}

#endif