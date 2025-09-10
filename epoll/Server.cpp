/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 17:40:20 by ndelhota          #+#    #+#             */
/*   Updated: 2025/09/08 13:34:16 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include "Server.hpp"
#include "Client.hpp"


volatile sig_atomic_t sigRes = 0;
// constructor/destructor
Server::Server(char *port, char *password): _port(std::atoi(port)), _password(password){
	if (_port < 1024 || _port > 49151 || strlen(port) > 5)
		throw invalidPort();
}

Server::Server(const Server& other): _port(other._port), _password(other._password){
}

Server::~Server( void ){

	std::map<int , Client*>::iterator	it;

	it = _ClientList.begin();
	for (it = _ClientList.begin(); it != _ClientList.end(); it++)
	{
		close(it->first);
		delete(it->second);
	}
	close(_serverSocket);
	close(epollFd);
}

// public

const char*	Server::invalidPort::what()const throw(){
	return("error, invalid server port number. Allowed port are located between [1024,49151]");
}

const char*	Server::launchError::what()const throw(){
	return ("error, server cannot be launched");
}

void		Server::launchServ(){
	_serverSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_port = htons(_port);
	_servAddr.sin_addr.s_addr = INADDR_ANY;
	bind(_serverSocket, (sockaddr*)&_servAddr, sizeof(_servAddr));
}

void	Server::acceptClient(void)
{
	int	clientfd;
	struct	epoll_event ev;
	Client* a;

	ev.events = EPOLLIN | EPOLLET;
	while ((clientfd = accept(_serverSocket, NULL, NULL)) != -1)
	{
		ev.data.fd = clientfd; //questionning on the split, might cause problem latter
		a = new Client(clientfd);
		_ClientList.insert(std::make_pair(clientfd, a));
		epoll_ctl(epollFd, EPOLL_CTL_ADD, clientfd, &ev); 
	}
	if (errno != EAGAIN && errno != EWOULDBLOCK)
		std::cerr << "error detected while trying to connect new client" << std::endl;
}

void	Server::addWriteFlag(Client* currentClient, int fd)
{
	uint32_t	 newFlags;
	struct		epoll_event ev;

	newFlags = EPOLLIN | EPOLLOUT | EPOLLET;
	if (newFlags != currentClient->getFlags())
	{
		ev.data.fd = fd;
		ev.events = newFlags;
		epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &ev);
	}
}

void	Server::removeWriteFlag(Client *currentClient, int fd)
{
	uint32_t	newFlags;
	struct		epoll_event ev;

	newFlags = EPOLLIN | EPOLLET;
	if (newFlags != currentClient->getFlags())
	{
		ev.events = newFlags;
		ev.data.fd = fd;
		epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &ev);
	}
}

void	Server::process_write(int fd)
{
	Client*	currentClient;

	currentClient = _ClientList[fd];
	if (currentClient->emptyWBuff())
		removeWriteFlag(currentClient, fd);
}

void	Server::removeClient(int fd)
{
	std::map<int, Client *>::iterator it;
	
	std::cout << "disconnection detected for the client using the fd: " << fd << std::endl;
	epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL);
	it = _ClientList.find(fd);
	close(fd);
	delete it->second;
	_ClientList.erase(it);
}
void	Server::process_read(int fd)
{
	Client* currentClient;

	currentClient = _ClientList[fd];
	//std::cout << "an event was catched for client designed by fd: " << fd <<  std::endl;
	char	BUFFER[1024];
	int		n;
	
	n = recv(fd, BUFFER, 1024, 0);
	if (n == 0)
	{
		removeClient(fd);
		return ;
	}
	if (!currentClient->emptyWBuff())
		addWriteFlag(currentClient, fd);
}

void		Server::SIGINT_handler(int)
{
		sigRes = SIGINT;
}

void		Server::signals_init(void)
{
	struct sigaction	mode;

	mode.sa_handler = &Server::SIGINT_handler;
	sigemptyset(&mode.sa_mask);
	mode.sa_flags = 0;
	sigaction(SIGINT, &mode, NULL);

}

void		Server::routine(void)
{
	struct	epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = _serverSocket;
	
	signals_init();
	listen(_serverSocket, 1000);
	epollFd = epoll_create(1); //size argument must be superior to 0 but is ignored since a linux patch
	epoll_ctl(epollFd, EPOLL_CTL_ADD, _serverSocket, &ev);
	while (!sigRes)
	{	
		struct epoll_event event_list[128];
		int n = epoll_wait(epollFd, event_list, 128, 10);
		for (int i = 0; i < n; i++)
		{
			ev = event_list[i];
			if (ev.data.fd == _serverSocket)
				acceptClient();
			else
			{
				if (ev.events & EPOLLIN)
					process_read(ev.data.fd);
				if (ev.events & EPOLLOUT)
					process_write(ev.data.fd);
				// if ((ev.events & EPOLLHUP) || (ev.events & EPOLLERR))
				// 	removeClient(ev.data.fd);
			}
		}
	}
}

