#include "Server.h"

namespace Irc {

	sig_atomic_t	sig_res = 0;

	bool 			Server::has_instance_ = false;
	// bool			Server::can_serve_ = false;
	const int		Server::DEFAULT_PORT = 6667;
	const int		Server::DEFAULT_PORT_TLS = 6697;
	const int		Server::QUEUE_SIZE = 64;

	/************************************************************
	*				🥚 CONSTRUCTORS & DESTRUCTOR				*
	************************************************************/

	Server::Server(int port, unsigned int hashed_password) : \
		port_(port), hashed_password_(hashed_password), server_fd_(-1), clients_(), clients_by_nick_(), client_connections_(), reply_factory_(HOST_NAME) {}

	Server::~Server(void)
	{
		close(epoll_fd_);
	}

	/************************************************************
	*				➕ OPERATORS									*
	************************************************************/
	
	std::ostream&	operator<<(std::ostream& os, Server& s)
	{
		return os << "Server [server_fd: " << s.get_server_fd() <<\
			" epoll_fd: " << s.get_epoll_fd() <<\
			" port: " << s.get_port() << "]" << std::endl;
	}

	/*************************************************************
	*		        🛠️ CLASS FUNCTIONS							*
	*************************************************************/

	void		Server::signals_init(void)
	{
		struct sigaction	mode;

		mode.sa_handler = &Server::handle_interrupt;
		sigemptyset(&mode.sa_mask);
		mode.sa_flags = 0;
		sigaction(SIGINT, &mode, NULL);
	}

	/*************************************************************
	*		        🛠️ FUNCTIONS								*
	*************************************************************/

	void	Server::accept_client()
	{
		int client_fd = accept(server_fd_, NULL, NULL);
		if (client_fd == -1)
		{
			Logger::error(std::string("accept error"));
		}
		Client* c = new Client(client_fd);
		clients_.insert(std::make_pair(client_fd, c));

		ClientConnection* co = new ClientConnection(client_fd);
		client_connections_.insert(std::make_pair(client_fd, co));
		// Server::set_non_blocking(client_fd);
		subscribe_to_event_(client_fd, EPOLLIN | EPOLLET);
	}

	void	Server::remove_client(int fd)
	{		
		Logger::info(std::string("removing client with fd ") + Utils::str(fd));
		epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL);
		Client* c = Server::get_client_by_fd(fd);
		std::string nick = c->get_nick();
		close(fd);
		delete c;
		clients_.erase(fd);
		clients_by_nick_.erase(nick);

		std::map<int, ClientConnection*>::iterator it = client_connections_.find(fd);
		delete it->second;
		client_connections_.erase(fd);
	}

	// what if Client send 2 cmds then disconnects - should we delete him that quickly ?
	void	Server::process_read(int client_fd, ClientConnection* co)
	{
		Logger::debug("Server#process_read");
		if (!co->receive())
			remove_client(client_fd);
		ACommand* cmd = CommandParser::parseCommand(co->get_read_buffer());
		if (cmd)
		{
			cmd->execute(*this, *co);
			delete cmd;
		}
		if (co->has_pending_write())
		{
			modify_event_(client_fd, EPOLLIN | EPOLLOUT);
		}
	}

	void	Server::process_write(int client_fd, ClientConnection* co)
	{
		Logger::debug("Server#process_write");
		if (!co->send_queue())
		{
			close(client_fd);
			client_connections_.erase(client_fd);
		}
		if (!co->has_pending_write())
			modify_event_(client_fd, EPOLLIN | EPOLLET);
	}

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
		if (sig == SIGINT)
		{
			Logger::debug("handle interrupting signal");
			sig_res = SIGINT;
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

		signals_init();
		Logger::debug(std::string("port = ") + port_str);
		int status = getaddrinfo(HOST_NAME.c_str(), port_str.c_str(), &hints, &result);
		if (status != 0)
		{
			Logger::error("getaddrinfo error");
			throw IRCException(SERVER_ERR, "addrinfo error");
		}
		Logger::debug(std::string("addr1 = ") + Utils::str(result->ai_addr));

		server_fd_ = -1;
		for (struct  addrinfo* test = result; test != NULL; test = test->ai_next)
		{
			server_fd_ = socket(test->ai_family, test->ai_socktype, test->ai_protocol);
			if (server_fd_ == -1)
				continue;
			fcntl(server_fd_, F_SETFL, O_NONBLOCK);
			int yes = 1;
			setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
			if (bind(server_fd_, test->ai_addr, test->ai_addrlen) == 0)
				break;
			close(server_fd_);
			server_fd_ = -1;
		};
		freeaddrinfo(result);

		if (listen(server_fd_, SOMAXCONN) == -1)
		{
			close(server_fd_);
			throw IRCException(SERVER_ERR, "listen error");
		}

		epoll_fd_ = epoll_create1(0);
		if (epoll_fd_ == -1)
		{
			close(server_fd_);
			throw IRCException(SERVER_ERR, "epoll create error");
		}
		subscribe_to_event_(server_fd_, EPOLLIN);
		// Server::can_serve_ = true;
		return server_fd_;
	}

	/// @brief
	/// @throw IRCException if error from accept
	void	Server::loop()
	{
		int 				client_fd;
		struct	epoll_event	ev;
		epoll_event events[QUEUE_SIZE];

		while (sig_res == 0)
		{
			int n = epoll_wait(epoll_fd_, events, Server::QUEUE_SIZE, -1);
			Logger::debug(std::string("nb of events: ") + Utils::str(n));

			if (n == -1)
			{
				Logger::error("epoll wait error");
				stop();
			}
			for (int i = 0; i < n; ++i)
			{
				ev = events[i];
				if (ev.data.fd == server_fd_)
					accept_client();
				else
				{
					client_fd = ev.data.fd;
					ClientConnection* co = client_connections_[client_fd];
				
					if (ev.data.fd & EPOLLIN)
					{
						process_read(client_fd, co);
					}
					if (ev.events & EPOLLOUT)
					{
						process_write(client_fd, co);
					}
					if ((ev.events & EPOLLHUP) || (ev.events & EPOLLERR))
						remove_client(ev.data.fd);
				}
			}

		}
	}

	void	Server::stop()
	{
		// Server::can_serve_ = false;
		if (server_fd_ != -1)
		{
			close(server_fd_);
			server_fd_ = -1;
		}
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

	Server	Server::get_instance(int port, unsigned int hashed_password)
	{
		static Server instance_(port, hashed_password);
		return instance_;
	}

	ReplyFactory&	Server::get_reply_factory()
	{
		return reply_factory_;
	}

	// bool	Server::can_serve()
	// {
	// 	return Server::can_serve_;
	// }

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

	// epoll_event*	Server::get_events() const
	// {
	// 	return events_;
	// }

}
