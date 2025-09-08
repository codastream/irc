#ifndef UTILS_H
#define UTILS_H

#include "ClientConnection.h"
#include "colors.h"

#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <map>

namespace Irc {
	bool			check_port(const std::string& s, int* port);
	bool			check_password(const std::string& s, unsigned int* hashed);
	unsigned int	simple_hash(const std::string& s);
}

namespace Utils {

	template<typename T>
	std::string		str(const T& value);

	template<typename T>
	std::string		str(const T& value)
	{
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}
}

#endif