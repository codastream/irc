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

	Server::Server(int port, unsigned int hashed_password) : \
		port_(port), hashed_password_(hashed_password), events_(new epoll_event[QUEUE_SIZE]), clients_(), clients_by_nick_(), client_connections_(), reply_factory_(HOST_NAME) {

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
	/// @throw exception if error in fcntl
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

	void	Server::modify_event_(int fd, uint32_t events)
	{
		epoll_event ev;
		ev.events = events;
		ev.data.fd = fd;
		epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev);
	}

	void	Server::handle_interrupt(int sig)
	{
		if (sig == SIGINT || sig == SIGTSTP)
		{
			Logger::debug("handle interrupting signal");
			Server::can_serve_ = false;
			delete Server::get_instance();
		}
	}

	/// @brief initializes a socket on default port and put it in listening mode
	/// @throw IRCException if socket() fails
	/// @return socket fd
	int		Server::start()
	{
		std::string port_str = Utils::str(port_);
		struct addrinfo hints = {};
		struct addrinfo* result = NULL;
		std::memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		Logger::debug(std::string("port = ") + port_str);
		int status = getaddrinfo(HOST_NAME.c_str(), port_str.c_str(), &hints, &result);
		if (status != 0)
		{
			Logger::error("getaddrinfo error");
			throw IRCException(SERVER_ERR, "addrinfo error");
		}
		Logger::debug(std::string("addr1 = ") + Utils::str(result->ai_addr));

		int server_fd = -1;
		for (struct  addrinfo* test = result; test != NULL; test = test->ai_next)
		{
			server_fd = socket(test->ai_family, test->ai_socktype, test->ai_protocol);
			if (server_fd == -1)
				continue;
			fcntl(server_fd_, F_SETFL, O_NONBLOCK);
			int yes = 1;
			setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
			if (bind(server_fd_, test->ai_addr, test->ai_addrlen) == 0)
				break;
			close(server_fd_);
			server_fd = -1;
		};
		freeaddrinfo(result);

		if (listen(server_fd_, SOMAXCONN) == -1)
		{
			close(server_fd);
			throw IRCException(SERVER_ERR, "listen error");
		}

		epoll_fd_ = epoll_create1(0);
		if (epoll_fd_ == -1)
		{
			close(server_fd);
			throw IRCException(SERVER_ERR, "epoll create error");
		}
		subscribe_to_event_(server_fd_, EPOLLIN);
		Server::can_serve_ = true;
		return server_fd_;
	}

	/// @brief 
	/// @throw IRCException if error from accept
	void	Server::loop()
	{
		while (Server::can_serve())
		{
			int n = epoll_wait(this->get_epoll_fd(), events_, Server::QUEUE_SIZE, -1);
			Logger::debug(std::string("nb of events: ") + Utils::str(n));

			if (n == -1)
			{
				this->stop();
			}
			for (int i = 0; i < n; ++i)
			{
				if (events_[i].data.fd == this->get_server_fd())
				{
					int client_fd = accept(this->get_server_fd(), NULL, NULL);
					if (client_fd == -1)
						throw IRCException(SERVER_ERR, "accept error");
					Client* c = new Client(client_fd);
					ClientConnection* co = new ClientConnection(client_fd);
					clients_.insert(std::make_pair(client_fd, c));
					client_connections_.insert(std::make_pair(client_fd, co));
					Server::set_non_blocking(client_fd);
					this->subscribe_to_event_(client_fd, EPOLLIN | EPOLLET);
				}
				else
				{
					int client_fd = events_[i].data.fd;
					ClientConnection* co = client_connections_.at(client_fd); // TODO check that clientConnection exists

					if (events_[i].events & EPOLLIN)
					{
						if (!co->receive())
						{
							Logger::error(std::string("receive error on fd " + Utils::str(co->get_fd())));
							close(client_fd);
							client_connections_.erase(client_fd);
							continue ;
						}
						ACommand* cmd = CommandParser::parseCommand(co->get_read_buffer());
						if (cmd)
						{
							cmd->execute(*this, *co);
							delete cmd;
						}
						if (co->has_pending_write())
						{
							this->modify_event_(client_fd, EPOLLIN | EPOLLOUT);
						}
					}

					if (events_[i].events & EPOLLOUT)
					{
						if (!co->send_queue())
						{
							close(client_fd);
							client_connections_.erase(client_fd);
							continue ;
						}
						if (!co->has_pending_write())
    						this->modify_event_(client_fd, EPOLLIN | EPOLLET);
					}
				}
			}	

		}
	}

	void	Server::stop()
	{
		Server::can_serve_ = false;
		if (server_fd_ != -1)
		{
			close(server_fd_);
			server_fd_ = -1;
		}
		delete Server::instance_;
	}

	bool	Server::is_valid_password(const std::string& test) const
	{
		return (simple_hash(test) == hashed_password_);
	}

	Client*		Server::get_client_by_fd(int client_fd)
	{
		std::map<int, Client*>::iterator it = clients_.find(client_fd);
		if (it != clients_.end())
			return it->second;
		return NULL;
	}

	Client*		Server::get_client_by_nick(const std::string& nick)
	{
		std::map<std::string, Client*>::iterator it = clients_by_nick_.find(nick);
		if (it != clients_by_nick_.end())
			return it->second;
		return NULL;
	}	

	void	Server::update_client_by_nick(Client* client)
	{
		clients_by_nick_[client->get_nick()] = client;
	}

	/*************************************************************
	*		    👁️‍ GETTERS and SETTERS				 			*
	*************************************************************/

	Server*	Server::get_instance()
	{
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