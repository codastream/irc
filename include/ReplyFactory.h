#ifndef REPLYFACTORY_H
#define REPLYFACTORY_H

#include "reply_codes.h"
#include "Client.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

namespace Irc {

class ReplyFactory {
	public:
		ReplyFactory(const std::string& server_name);
		virtual ~ReplyFactory();

		std::string	make_reply(ReplyCode code, const std::string& target, const std::string& trailing_msg);
		std::string	make_reply(ReplyCode code, const std::string& target, \
			std::vector<std::string>& params, const std::string& trailing_msg);
		
	private:
		std::string	server_name_;
};

}

#endif