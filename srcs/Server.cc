#include "Server.h"

namespace Irc {

	Server* 		Server::instance_ = NULL;
	bool			Server::can_serve_ = false;
	const int		Server::DEFAULT_PORT = 6667;
	const int		Server::DEFAULT_PORT_TLS = 6697;
	const int		Server::QUEUE_SIZE = 64;
	const int 		Server::MESSAGE_SIZE = 512;

/************************************************************
*				🥚 CONSTRUCTORS & DESTRUCTOR				*
************************************************************/

Server::Server(void) : \
	port_(DEFAULT_PORT), hashed_password_(0), events_(new epoll_event[QUEUE_SIZE]) {}

Server::Server(int port, unsigned int hashed_password) : \
	port_(port), hashed_password_(hashed_password), events_(new epoll_event[QUEUE_SIZE]) {

		instance_ = this;
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

void	Server::subscribe_to_event_(int fd, uint32_t events)
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
		Server::can_serve_ = false;
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
	subscribe_to_event_(server_fd_, EPOLLIN);
	Server::can_serve_ = true;
	return server_fd_;
}

void	Server::loop()
{
	while (Server::can_serve())
	{
		int n = epoll_wait(this->get_epoll_fd(), events_, Irc::Server::QUEUE_SIZE, -1);
		if (n == -1)
		{
			this->stop();
		}
		for (int i = 0; i < n; ++i)
		{
			if (events_[i].data.fd == this->get_server_fd())
			{
				int client_fd = accept(this->get_server_fd(), NULL, NULL);
				Irc::Server::set_non_blocking(client_fd);
				this->subscribe_to_event_(client_fd, EPOLLIN | EPOLLET);
			}
			else
			{
				char readBuf[MESSAGE_SIZE];
				errno = 0;
				ssize_t len = recv(events_[i].data.fd, readBuf, sizeof(readBuf), 0);
				if (len == -1)
				{
					std::cerr << "rcv error" << std::endl;
					close(events_[i].data.fd);
				}
				else if (len == 0)
				{
					std::cout << "EOF" << std::endl;
					close(events_[i].data.fd);
				}
				else
				{
					readBuf[len] = '\0';
					std::cout << "msg: " << readBuf << std::endl;
				}
			}
		}
	}
}

void	Server::stop()
{
	Server::can_serve_ = false;
}

/*************************************************************
*		    👁️‍ GETTERS and SETTERS				 			*
*************************************************************/

Server*	Server::get_instance()
{
	if (!Server::instance_)
	{
		return new Server(DEFAULT_PORT, 0);
	}
	return instance_;
}

Server*	Server::get_instance(int port, unsigned int hashed_password)
{
	if (!Server::instance_)
	{
		instance_ = new Server(port, hashed_password);
	}
	return instance_;
}

bool	Server::can_serve()
{
	return Server::can_serve_;
}

bool	Server::is_valid_password(const std::string& test)
{
	return (simple_hash(test) == hashed_password_);
}

int	Server::get_port()
{
	return port_;
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