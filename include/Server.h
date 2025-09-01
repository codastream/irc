#ifndef SERVER_H
#define SERVER_H

#include "Client.h"

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>

#include "utils.h"

namespace Irc {
	class Server {
		public:
			~Server();

			static Server*	getInstance(); // singleton
			
			int 			start();
			void 			stop();

		private:
			int					server_fd_;
			int					port_;
			std::vector<Client> clients_;
			static Server*		instance_;
			static const int	DEFAULT_PORT;

			Server();
			Server(const Server& other);
			Server& 		operator=(const Server& other);
	};
}

#endif