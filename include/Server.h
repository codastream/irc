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
			static const int			QUEUE_SIZE;

			static Server*	get_instance(); // singleton
			static void		handle_interrupt(int sig);
			static int		set_non_blocking(int fd);

			bool			can_serve();
			int				get_epoll_fd();
			int				get_server_fd();
			epoll_event*	get_events();
	
			int 			start();
			void 			stop();
			void			subscribe_to_event(int fd, uint32_t events);

		private:


			bool						can_serve_;
			int							server_fd_;
			int							epoll_fd_;
			std::vector<Client> 		clients_;
			epoll_event*				events_;
			static Server*				instance_;

			Server();
	};
}

#endif