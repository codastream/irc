#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>

namespace Irc {

	enum ClientStatus
	{
		UNAUTHENTICATED,
		AUTHENTICATED,		// after PASS
		REGISTERED,			// after PASS, NICK and USER
	};
	class Client
	{
		public:
			Client(int fd);
			virtual ~Client();
			Client(const Client& other);
			Client& operator=(const Client& other);

			int				get_fd() const;
			ClientStatus	get_status() const;
			void			set_status(ClientStatus status);
			std::string		get_nick() const;
			void			set_nick(const std::string& nick);
			bool			is_active() const;

		private:
			int				client_fd_;
			ClientStatus	status_;
			std::string		nickname_;
			std::string		username_;
			std::string		opname_;
			bool			isOperator_;

			Client();

	};
}

#endif