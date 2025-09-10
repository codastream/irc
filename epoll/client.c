/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 12:36:53 by ndelhota          #+#    #+#             */
/*   Updated: 2025/09/10 12:56:08 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/wait.h>

void	ft_client(void)
{
	struct sockaddr_in	server;
	int 		sfd;
	     
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_port = htons(1200);
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
	connect(sfd, (struct sockaddr*)&server, sizeof(server));
	usleep(3000000);
	close(sfd);
	exit(0);
}

int	main(int ac, char **argv)
{
	int	nb;
	pid_t	pid;
	
	if (ac != 2)
		return (0);
	nb = atoi(*(++argv));
	while (nb--)
	{
		pid = fork();
		if (!pid)
			ft_client();
	}
	while (nb < (ac - 1))
	{
		waitpid(0, NULL, 0);
		nb++;
	}
}
