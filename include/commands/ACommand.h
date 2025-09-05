#ifndef COMMAND_H
#define COMMAND_H

#include "Server.h"
#include "Client.h"
#include "Message.h"

#include <string>
#include <vector>

namespace Irc {

class ACommand {
	public:
		ACommand();
		ACommand(std::vector<std::string> args);
		ACommand(const ACommand& other);
		virtual ~ACommand();
		ACommand& operator=(const ACommand& other);
		
		virtual void execute(Server& s, Irc::Client& c, Irc::Message& m) = 0;

	private:
		std::vector<std::string> args_;
};

}

#endif