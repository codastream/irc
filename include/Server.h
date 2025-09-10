#ifndef SERVER_H
#define SERVER_H

#include "Client.h"
#include "ClientConnection.h"
#include "CommandParser.h"
#include "IRCException.h"
#include "ReplyFactory.h"

#include "Logger.h"
#include "utils.h"
#include "config.h"

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <cstring>					// memset
#include <sys/socket.h>
#include <sys/types.h>				
#include <sys/epoll.h>
#include <netdb.h>					// adrrinfo
#include <netinet/in.h>				// sockaddr_in6
#include <signal.h>
#include <fcntl.h>
#include <errno.h>					// is it allowed ?
#include <unistd.h>					// close

#define DEBUG 0

namespace Irc {

	class Server {
		public:
			~Server();

			static const int			DEFAULT_PORT;
			static const int			DEFAULT_PORT_TLS;
			static const int			QUEUE_SIZE;
			static const int			MESSAGE_SIZE;

			static Server*	get_instance();
			static Server*	get_instance(int port, unsigned int hashed_password); // singleton
			static void		handle_interrupt(int sig);
			static int		set_non_blocking(int fd);
			static bool		can_serve();
		
			Client*			get_client_by_fd(int client_fd);
			Client*			get_client_by_nick(const std::string& nick);
			void			update_client_by_nick(Client* client);
			
			ReplyFactory&	get_reply_factory();
			int				get_port() const;
			int				get_epoll_fd() const;
			int				get_server_fd() const;
			epoll_event*	get_events() const;
	
			int 			start();
			void			loop();
			void 			stop();
			bool			is_valid_password(const std::string& test) const;

		private:
			
			static bool							can_serve_;
			static Server*						instance_;

			int									port_;
			unsigned int						hashed_password_;
			int									server_fd_;
			int									epoll_fd_;
			epoll_event*						events_;
			std::map<int, Client*> 				clients_;
			std::map<std::string, Client*> 		clients_by_nick_;
			std::map<int, ClientConnection*> 	client_connections_;
			ReplyFactory						reply_factory_;

			// Server();
			Server(int port, unsigned int hashed_password);

			void			subscribe_to_event_(int fd, uint32_t events);
			void			modify_event_(int fd, uint32_t events);

	};

	std::ostream&	operator<<(std::ostream& os, Server& s);
}

namespace Utils {

	template <typename T>
	void	delete_map(std::map<int, T*>& map, bool should_close_fd=false)
	{
		for (typename std::map<int, T*>::iterator it = map.begin(); it != map.end(); ++it)
		{
			if (should_close_fd)
			{
				close(it->first);
			}
		}
		map.clear();
	}
}

#endif