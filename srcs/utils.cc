#include "utils.h"

namespace Irc {

	bool	check_port(const std::string& s, int* port)
	{
		char* end;
		long n = std::strtol(s.c_str(), &end, 10);
		if (!(n > 1024 && n < 49152))
			return false;
		*port = n;
		return true;
	}

	/// @brief simple hash which doesn't guarantee absence of collision. Yet better than storing password in clear
	/// @param s 
	/// @return 
	unsigned int	simple_hash(const std::string& s)
	{
		unsigned int hash = 5381;
		for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
		{
			hash = ((hash << 5) + hash) ^ (*it);
		}
		return hash;
	}

	bool	check_password(const std::string& s, unsigned int* hashed)
	{
		if (s.length() < 8)
		{
			std::cerr << RED << "password must be 8 characters long at least" << NC << std::endl;
			return false;
		}
		*hashed = simple_hash(s);
		return true;
	}
}
