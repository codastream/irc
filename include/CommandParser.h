#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include "commands/ACommand.h"
#include "commands/CommandFactory.h"
#include "IRCException.h"

#include <string>
#include <sstream>
#include <vector>

class ACommand;
class Server;

namespace Irc {

	class CommandParser {
		public:
			CommandParser();
			virtual ~CommandParser();

			static ACommand* parseCommand(const std::string& msg);
			
		private:
			struct Message
			{
				std::string command_name;
				std::vector<std::string> params;		
			};

			bool	check_format_(const std::string& msg);
	};

}

#endif