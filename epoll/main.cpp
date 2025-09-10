/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:54:26 by ndelhota          #+#    #+#             */
/*   Updated: 2025/09/07 12:08:01 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	main(int ac, char **argv)
{
	if (ac != 3){
		std::cout << "invalid argument number";
		return 0;
	}
	try{
		Server* a = new Server(argv[1], argv[2]);
		a->launchServ();
		a->routine();
		delete(a);
	}
	catch(std::exception &e){
		std::cerr << e.what() << std::endl;
	}
}
