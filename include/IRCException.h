#ifndef IRCEXCEPTION_H
#define IRCEXCEPTION_H

#include <string>
#include <exception>

namespace Irc {
	class IRCException : public std::exception {
		public:

			IRCException(const std::string& msg);
			virtual ~IRCException() throw();
			
			virtual const char* what() const throw();
			
		private:

			std::string	message_;

	};
}

#endif