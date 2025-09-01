#include "Server.h"

namespace Irc {

	Server* 		Server::instance_ = NULL;
	const int		Server::DEFAULT_PORT = 6697;

/************************************************************
*				🥚 CONSTRUCTORS & DESTRUCTOR				*
************************************************************/

Server::Server(void) : port_(DEFAULT_PORT) {}

Server::Server(const Server& other) : port_(other.port_) {}

Server::~Server(void) 
{
	if (instance_)
		delete instance_;
}

/************************************************************
*				➕ OPERATORS									*
************************************************************/

Server& 		Server::operator=(const Server& other)
{
	if (&other != this)
		port_ = other.port_;
	return *this;
}

/*************************************************************
*		        🛠️ FUNCTIONS								*
*************************************************************/

int		Server::start()
{
	int fd = socket(AF_INET6, SOCK_STREAM, 0);
	server_fd_ = fd;
	return fd;
}

/*************************************************************
*		    👁️‍ GETTERS and SETTERS				 			*
*************************************************************/

Server*	Server::getInstance()
{
	if (!Server::instance_)
	{
		instance_ = new Server();
	}
	return instance_;
}

}