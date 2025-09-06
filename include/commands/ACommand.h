#ifndef COMMAND_H
#define COMMAND_H

#include "ClientConnection.h"
#include "IRCException.h"

#include <iostream>
#include <string>
#include <vector>

namespace Irc {

	class Server;

	class ACommand {
		public:
			ACommand();
			ACommand(std::vector<std::string> args);
			ACommand(const ACommand& other);
			virtual ~ACommand();
			ACommand& operator=(const ACommand& other);
			
			virtual void execute(Irc::Server& s, ClientConnection& co) = 0;

		protected:
			std::vector<std::string> args_;
	};

}

#endif