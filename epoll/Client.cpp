#include "Client.hpp"
#include <cstdlib>
#include <unistd.h>
#include <sys/epoll.h>

// constructor/destructor

Client::Client(int fd){\
	clientFd = fd;
	activeFlags = EPOLLIN | EPOLLET;
	std::cout << "created a client communicating throught fd number: " << fd << std::endl;
}

Client::Client(const Client& other){
	clientFd = other.clientFd;
}

Client::~Client( void ){
	close(clientFd);
}


// public

bool	Client::emptyWBuff(void){
	return W_BUFFER.empty();
}

uint32_t	Client::getFlags(void){
	return activeFlags;
}

int	Client::getFd(void){
	return clientFd;
}

Client&	Client::operator =(const Client& other){
	if (this != &other){
		clientFd = other.clientFd;
	}
	return (*this);
}
//private

//protected

