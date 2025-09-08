#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include "IRCException.h"
#include "ReplyFactory.h"

#include "reply_codes.h"
#include "config.h"

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

			std::string		get_read_buffer() const;
			std::string		get_write_buffer() const;
			// ReplyFactory&	get_reply_factory() const;
			int				get_fd() const;
			bool			has_pending_write() const;
			void			queue_reply(const std::string& msg);
			bool			receive();
			bool			send_queue();

			void			queue_err_needmoreparams(const std::string& cmd_name);
			void			queue_err_alreadyregistered(const std::string& nick);
			void			queue_err_passwdmismatch();
			void			queue_err_nonicknamegiven();
			void			queue_err_erroneusnickname(const std::string& nick);
			void			queue_err_nicknameinuse(const std::string& nick);

		private:
			int				fd_;
			std::string		read_buffer_;
			std::string		write_buffer_;
			bool			has_pending_write_;
			ReplyFactory	rf_;
	};

}

#endif