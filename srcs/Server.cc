#include "Server.h"

namespace Irc {

	Server* 		Server::instance_ = NULL;
	bool			Server::can_serve_ = false;
	const int		Server::DEFAULT_PORT = 6667;
	const int		Server::DEFAULT_PORT_TLS = 6697;
	const int		Server::QUEUE_SIZE = 64;

	/************************************************************
	*				🥚 CONSTRUCTORS & DESTRUCTOR				*
	************************************************************/

	// Server::Server(void) : \
	// 	port_(DEFAULT_PORT), hashed_password_(0), events_(new epoll_event[QUEUE_SIZE]), host_name_("server"), clients_(), client_connections_(), reply_factory_(host_name_) {}

	Server::Server(int port, unsigned int hashed_password) : \
		port_(port), hashed_password_(hashed_password), events_(new epoll_event[QUEUE_SIZE]), host_name_("server"), clients_(), client_connections_(), reply_factory_(host_name_) {

			instance_ = this;
	}

	Server::~Server(void) 
	{
		delete[] events_;
		Utils::delete_map(clients_);
		Utils::delete_map(client_connections_, true);
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
			throw IRCException(FORMAT_ERR, "error getting fd flags");
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
			throw IRCException(SERVER_ERR, "socket init error");
		sockaddr_in6 addr;
		std::memset(&addr, 0, sizeof(addr));
		addr.sin6_family = AF_INET6;
		addr.sin6_port = htons(Server::DEFAULT_PORT);
		addr.sin6_addr = in6addr_any;
		if (bind(server_fd_, (sockaddr*)&addr, sizeof(addr)) == -1)
			throw IRCException(SERVER_ERR, "bind error");
		if (listen(server_fd_, SOMAXCONN) == -1)
			throw IRCException(SERVER_ERR, "listen error");

		epoll_fd_ = epoll_create1(0);
		if (epoll_fd_ == -1)
			throw IRCException(SERVER_ERR, "epoll create error");
		subscribe_to_event_(server_fd_, EPOLLIN);
		Server::can_serve_ = true;
		return server_fd_;
	}

	void	Server::loop()
	{
		while (Server::can_serve())
		{
			int n = epoll_wait(this->get_epoll_fd(), events_, Server::QUEUE_SIZE, -1);
			if (n == -1)
			{
				this->stop();
			}
			for (int i = 0; i < n; ++i)
			{
				if (events_[i].data.fd == this->get_server_fd())
				{
					int client_fd = accept(this->get_server_fd(), NULL, NULL);
					// Client* c = new Client(client_fd);
					ClientConnection* co = new ClientConnection(client_fd);
					// clients_.insert(std::make_pair(client_fd, c));
					client_connections_.insert(std::make_pair(client_fd, co));
					Server::set_non_blocking(client_fd);
					this->subscribe_to_event_(client_fd, EPOLLIN | EPOLLET);
				}
				
				int client_fd = events_[i].data.fd;
				ClientConnection* co = client_connections_.at(client_fd);

				if (events_[i].events & EPOLLIN)
				{
					if (!co->receive())
					{
						close(client_fd);
						client_connections_.erase(client_fd);
						continue ;
					}
					ACommand* cmd = CommandParser::parseCommand(co->get_read_buffer());
					cmd->execute(*this, *co);
				}

				if (events_[i].events & EPOLLOUT)
				{
					if (!co->send_pending())
					{
						close(client_fd);
						client_connections_.erase(client_fd);
						continue ;
					}
				}
			}
		}
	}

	void	Server::stop()
	{
		Server::can_serve_ = false;
	}

	bool	Server::is_valid_password(const std::string& test) const
	{
		return (simple_hash(test) == hashed_password_);
	}

	Client&		Server::get_client_by_fd(int client_fd)
	{
		std::map<int, Client*>::iterator it = clients_.find(client_fd);
		if (it == clients_.end())
			throw std::runtime_error("client not found");
		return *(it->second);
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

	ReplyFactory&	Server::get_reply_factory()
	{
		return reply_factory_;
	}

	bool	Server::can_serve()
	{
		return Server::can_serve_;
	}

	int	Server::get_port() const
	{
		return port_;
	}

	int		Server::get_epoll_fd() const
	{
		return epoll_fd_;
	}

	int		Server::get_server_fd() const
	{
		return server_fd_;
	}

	epoll_event*	Server::get_events() const
	{
		return events_;
	}

}