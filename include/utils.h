#ifndef UTILS_H
#define UTILS_H

#include "ClientConnection.h"
#include "colors.h"

#include <cstdlib>
#include <string>
#include <iostream>
#include <map>

namespace Irc {
	bool			check_port(const std::string& s, int* port);
	bool			check_password(const std::string& s, unsigned int* hashed);
	unsigned int	simple_hash(const std::string& s);
}

#endif