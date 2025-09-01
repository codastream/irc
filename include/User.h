#ifndef USER_H
#define USER_H

#include "Client.h"

#include <iostream>
#include <string>

namespace Irc {
class User : public Client
{
	public:
		User();
		virtual ~User();
		User(const User& other);

		User& operator=(const User& other);

		const std::string&	getNickname() const;
		bool				isActive() const;

		static User&		getUserByFd(int client_fd);

	private:
		std::string		nickname_;
		std::string		username_;
		std::string		opname_;
		bool			isOperator_;
		bool			isRegistered_;
};
}

#endif