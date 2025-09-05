#ifndef SERVER_H
#define SERVER_H

#include "Client.h"
#include "IRCException.h"

#include <iostream>
#include <string>
#include <vector>
#include <cstring>					// memset
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>				// sockaddr_in6
#include <signal.h>
#include <fcntl.h>
#include <errno.h>					// is it allowed ?

#include "utils.h"

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

			int				get_port();
			int				get_epoll_fd();
			int				get_server_fd();
			epoll_event*	get_events();
	
			int 			start();
			void			loop();
			void 			stop();
			bool			is_valid_password(const std::string& test);

		private:
			
			static bool					can_serve_;
			static Server*				instance_;

			int							port_;
			unsigned int				hashed_password_;
			epoll_event*				events_;
			int							server_fd_;
			int							epoll_fd_;
			std::vector<Client> 		clients_;

			Server();
			Server(int port, unsigned int hashed_password);

			void			subscribe_to_event_(int fd, uint32_t events);

	};
}

#endif