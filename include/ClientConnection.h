#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include "IRCException.h"
#include "reply_codes.h"

#include <iostream>
#include <string>
#include <sys/socket.h>		// recv
#include <errno.h>			// authorized ?

namespace Irc {

	class ClientConnection {

		static const int	BUFFER_SIZE;

		public:
			ClientConnection(int client_fd);
			virtual ~ClientConnection();
			ClientConnection(const ClientConnection& other);

			std::string	get_read_buffer() const;
			std::string	get_write_buffer() const;
			int			get_fd() const;
			bool		has_pending_write() const;
			void		add_reply(const std::string& msg);
			bool		receive();
			bool		send_pending();

		private:
			int			fd_;
			std::string	read_buffer_;
			std::string	write_buffer_;
			bool		has_pending_write_;
	};

}

#endif