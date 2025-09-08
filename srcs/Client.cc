#include "Client.h"

namespace Irc {

	/************************************************************
	*		🥚 CONSTRUCTORS & DESTRUCTOR				*
	************************************************************/

	Client::Client(void) : client_fd_(-1), status_(UNAUTHENTICATED) {}

	Client::Client(int fd) : client_fd_(fd), status_(UNAUTHENTICATED) {}

	Client::Client(const Client& inst) : client_fd_(inst.client_fd_), status_(inst.status_) {}

	Client::~Client(void) {}

	/************************************************************
	*		➕ OPERATORS									*
	************************************************************/

	Client& Client::operator=(const Client& inst) 
	{
		if (this != &inst)
		{
			client_fd_ = inst.client_fd_;
			status_ = inst.status_;
		}
		return (*this);
	}

	/*************************************************************
	*		🛠️ FUNCTIONS								*
	*************************************************************/

	/*************************************************************
	*		👁️‍ GETTERS and SETTERS				 			*
	*************************************************************/

	int	Client::get_fd() const
	{
		return client_fd_;
	}

	ClientStatus Client::get_status() const
	{
		return status_;
	}

	void	Client::set_status(ClientStatus status)
	{
		status_ = status;
	}

	std::string Client::get_nick() const
	{
		return nickname_;
	}

	void Client::set_nick(const std::string& nick)
	{
		nickname_ = nick;
	}

}