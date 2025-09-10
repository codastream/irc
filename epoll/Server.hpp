/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:17:56 by ndelhota          #+#    #+#             */
/*   Updated: 2025/09/08 12:30:02 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <map>
class Client;

class Server{
	public:
		Server(char* port, char* password);
		Server(const Server& other);
		virtual ~Server( void );
		void	routine(void);
		void	launchServ( void );
		//exception
		class invalidPort : public std::exception{
			virtual const char* what()const throw();	
		};
		class launchError : public std::exception{
			virtual const char* what()const throw();
		};
	private:
		Server&		operator =(const Server& other);
		static void	SIGINT_handler(int);
		void		signals_init(void);
		void		acceptClient(void);
		void		process_read(int fd);
		void		process_write(int fd);
		void		addWriteFlag(Client* currentClient, int fd);
		void		removeWriteFlag(Client* currentClient, int fd);
		void		removeClient(int fd);
		const int	_port;
		char*		_password;
		int		_serverSocket;
		int		epollFd;
		sockaddr_in	_servAddr;
		std::map<int, Client *>	_ClientList;
	protected:

};
