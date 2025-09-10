#include "Server.h"
#include "colors.h"
#include "utils.h"

#include <iostream>

namespace Irc {

	bool	check_args(int ac, char** av, int* port, unsigned int* hashed)
	{
		if (ac != 3)
		{
			std::cerr << RED << "usage: irc <port> <password>" << NC << std::endl;
			return false;
		}
		if (!check_port(av[1], port))
			return false;
		if (!check_password(av[2], hashed))
			return false;
		return true;
	}
}

int main(int ac, char** av)
{
	int				port;
	unsigned int	hashed;

	if (!Irc::check_args(ac, av, &port, &hashed))
		return EXIT_FAILURE;
	Irc::Server s = Irc::Server::get_instance(port, hashed);
	signal(SIGINT, Irc::Server::handle_interrupt);
	signal(SIGTSTP, Irc::Server::handle_interrupt);
	
	try
	{
		s.start();
		s.loop();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}