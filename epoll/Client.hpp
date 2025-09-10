/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:17:55 by ndelhota          #+#    #+#             */
/*   Updated: 2025/09/08 12:31:14 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include "Server.hpp" 

class Client{
	public:
		Client( void );
		Client(const Client& other);
		virtual ~Client( void );
		Client(int fd);
		Client&	operator =(const Client& other);
		bool	emptyWBuff( void );
		uint32_t	getFlags( void );
		int		getFd( void );
	private:
		std::string 	R_BUFFER;
		std::string	W_BUFFER;
		int		clientFd;
		uint32_t	activeFlags;
	protected:

};
