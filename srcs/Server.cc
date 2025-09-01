#include "Server.h"

namespace Irc {

	Server* 		Server::instance_ = NULL;
	const int		Server::DEFAULT_PORT = 6697;
	const int		Server::QUEUE_SIZE = 64;

/************************************************************
*				🥚 CONSTRUCTORS & DESTRUCTOR				*
************************************************************/

Server::Server(void) : events_(new epoll_event[64]) {
}

Server::~Server(void) 
{
	delete[] events_;
}

/************************************************************
*				➕ OPERATORS									*
************************************************************/

/*************************************************************
*		        🛠️ FUNCTIONS								*
*************************************************************/

/// @brief 
/// @param fd 
/// @throw exception if fcntl fails
/// @return 
int	Server::set_non_blocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		throw IRCException("error getting fd flags");
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void	Server::subscribe_to_event(int fd, uint32_t events)
{
	epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
}

void	Server::handle_interrupt(int sig)
 {
	if (sig == SIGINT || sig == SIGTSTP)
	{
		if (DEBUG)
			std::cout << "handle interrupt" << std::endl;
		Server *s = Server::get_instance();
		s->can_serve_ = false;
	}
 }

/// @brief initializes a socket on default port and put it in listening mode
/// @throw IRCException if socket() fails
/// @return socket fd
int		Server::start()
{
	server_fd_ = socket(AF_INET6, SOCK_STREAM, 0);
	if (server_fd_ == -1)
		throw IRCException("socket init error");
	sockaddr_in6 addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(Server::DEFAULT_PORT);
	addr.sin6_addr = in6addr_any;
	if (bind(server_fd_, (sockaddr*)&addr, sizeof(addr)) == -1)
		throw IRCException("bind error");
	if (listen(server_fd_, SOMAXCONN) == -1)
		throw IRCException("listen error");

	epoll_fd_ = epoll_create1(0);
	if (epoll_fd_ == -1)
		throw IRCException("epoll create error");
	subscribe_to_event(server_fd_, EPOLLIN);
	can_serve_ = true;
	return server_fd_;
}

void	Server::stop()
{
	can_serve_ = false;
}

/*************************************************************
*		    👁️‍ GETTERS and SETTERS				 			*
*************************************************************/

Server*	Server::get_instance()
{
	if (!Server::instance_)
	{
		instance_ = new Server();
	}
	return instance_;
}

bool	Server::can_serve()
{
	return can_serve_;
}

int		Server::get_epoll_fd()
{
	return epoll_fd_;
}

int		Server::get_server_fd()
{
	return server_fd_;
}

epoll_event*	Server::get_events()
{
	return events_;
}

}