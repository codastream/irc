#include "ClientConnection.h"

namespace Irc {

	const int 		ClientConnection::BUFFER_SIZE = 4096;

	/************************************************************
	*		🥚 CONSTRUCTORS & DESTRUCTOR						*
	************************************************************/

	ClientConnection::ClientConnection(int client_fd) : fd_(client_fd), read_buffer_(), \
		write_buffer_(), has_pending_write_(false), rf_(HOST_NAME) {}

	ClientConnection::~ClientConnection(void) {}

	/************************************************************
	*		➕ OPERATORS											*
	************************************************************/

	std::ostream&	operator<<(std::ostream& os, ClientConnection& co)
	{
		return os << "Connection [fd: " << co.get_fd() << "\n"\
			" toWrite: " << co.get_write_buffer() << "\n"\
			" toRead: " << co.get_read_buffer() << "]" << std::endl;
	}

	/*************************************************************
	*		🛠️ FUNCTIONS										*
	*************************************************************/

	void		ClientConnection::queue_reply(const std::string& msg)
	{
		write_buffer_ += msg;
	}
	// TODO check if better to return exception on failed read and send
	/// @brief fills read buffer
	/// @return false if nothing to read
	/// @throw exception if fd not properly closed
	bool		ClientConnection::receive()
	{
		char buffer[ClientConnection::BUFFER_SIZE];

		while (true)
		{
			ssize_t nb_read = recv(fd_, buffer, sizeof(buffer), 0);
			Logger::debug("ClientConnection#receive - buffer = ", buffer);
			if (nb_read > 0)
				read_buffer_.append(buffer, nb_read);
			else if (nb_read == 0)
			{
				Logger::debug(std::string("client gracefully disconnected"));
				return false;
			}
			else
			{
				Logger::error("recv error");
				return false;
			}
		}
		return true;
	}

	/// @brief empties the write buffer into client fd
	/// @throw exception if fd not properly closed
	bool		ClientConnection::send_queue()
	{
		while (!write_buffer_.empty())
		{
			ssize_t nb_sent = send(fd_, write_buffer_.c_str(), write_buffer_.size(), 0);	
			if (nb_sent == -1)
			{
				Logger::error("send error");
			}
			write_buffer_.erase(0, nb_sent);
		}
		return true;
	}

	void	ClientConnection::queue_err_needmoreparams(const std::string& cmd_name)
	{
		std::string msg = rf_.make_reply(ERR_NEEDMOREPARAMS, cmd_name, "Not enough parameters");
		queue_reply(msg);
	}

	void	ClientConnection::queue_err_alreadyregistered(const std::string& nick)
	{
		std::string msg = rf_.make_reply(ERR_ALREADYREGISTERED, nick, "Unauthorized command (already registered)");
		queue_reply(msg);
	}

	void	ClientConnection::queue_err_passwdmismatch()
	{
		std::string msg = rf_.make_reply(ERR_PASSWDMISMATCH, "*", "Password incorrect");
		queue_reply(msg);
	}

	void	ClientConnection::queue_err_nonicknamegiven()
	{
		std::string msg = rf_.make_reply(ERR_NONICKNAMEGIVEN, "*", "No nickname given");
		queue_reply(msg);
	}

	void	ClientConnection::queue_err_erroneusnickname(const std::string& nick)
	{
		std::string msg = rf_.make_reply(ERR_ERRONEUSNICKNAME, nick, "Erroneous nickname");
		queue_reply(msg);
	}

	void	ClientConnection::queue_err_nicknameinuse(const std::string& nick)
	{
		std::string msg = rf_.make_reply(ERR_NICKNAMEINUSE, nick, "Nickname is already in use");
		queue_reply(msg);
	}

	/*************************************************************
	*		👁️‍ GETTERS and SETTERS				 				*
	*************************************************************/

	std::string	ClientConnection::get_read_buffer() const
	{
		return read_buffer_;
	}

	std::string	ClientConnection::get_write_buffer() const
	{
		return write_buffer_;
	}

	bool		ClientConnection::has_pending_write() const
	{
		return !write_buffer_.empty();
	}

	int			ClientConnection::get_fd() const
	{
		return fd_;
	}


}
