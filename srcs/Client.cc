#include "Client.h"

namespace Irc {
/************************************************************
*		🥚 CONSTRUCTORS & DESTRUCTOR				*
************************************************************/

Client::Client(void) : client_fd_(-1), client_buffer_(NULL) {}

Client::Client(const Client& inst) : client_fd_(inst.client_fd_), client_buffer_(inst.client_buffer_) {}

Client::~Client(void) {}

/************************************************************
*		➕ OPERATORS									*
************************************************************/

Client& Client::operator=(const Client& inst) 
{
	if (this != &inst)
	{
		client_fd_ = inst.client_fd_;
		client_buffer_ = inst.client_buffer_;
	}
	return (*this);
}

/*************************************************************
*		🛠️ FUNCTIONS								*
*************************************************************/

/*************************************************************
*		👁️‍ GETTERS and SETTERS				 			*
*************************************************************/

}