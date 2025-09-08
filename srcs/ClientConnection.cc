#include "ClientConnection.h"

namespace Irc {

	const int 		ClientConnection::BUFFER_SIZE = 4096;

	/************************************************************
	*		🥚 CONSTRUCTORS & DESTRUCTOR						*
	************************************************************/

	ClientConnection::ClientConnection(int client_fd) : fd_(client_fd), read_buffer_(), write_buffer_(), has_pending_write_(false) {}

	ClientConnection::~ClientConnection(void) {}

	/************************************************************
	*		➕ OPERATORS											*
	************************************************************/

	/*************************************************************
	*		🛠️ FUNCTIONS										*
	*************************************************************/

	void		ClientConnection::queue_reply(const std::string& msg)
	{
		std::cout << "adding " << msg << " to write buffer" << std::endl;
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
			errno = 0;
			ssize_t nb_read = recv(fd_, buffer, sizeof(buffer), 0);
			if (nb_read > 0)
				read_buffer_.append(buffer, nb_read);
			else if (nb_read == 0)
				return false;
			else
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK) // saturated socket
					break ;
				throw IRCException(SERVER_ERR);
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
			errno = 0;
			ssize_t nb_sent = send(fd_, write_buffer_.c_str(), write_buffer_.size(), 0);	
			std::cout << "sending " << write_buffer_ << std::endl;
			if (nb_sent == -1)
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK) // saturated socket
					return true;
				throw IRCException(SERVER_ERR);
			}
			write_buffer_.erase(0, nb_sent);
		}
		return true;
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
		std::cout << "write buffer: " << write_buffer_ << std::endl;
		return !write_buffer_.empty();
	}

	int			ClientConnection::get_fd() const
	{
		return fd_;
	}

}
