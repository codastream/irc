#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>

namespace Irc {
class Client
{
	public:
		Client();
		virtual ~Client();
		Client(const Client& other);
		Client& operator=(const Client& other);
		
	private:
		int		client_fd_;
		char*	client_buffer_;

};
}

#endif