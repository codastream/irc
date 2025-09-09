#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include "commands/PassCommand.h"
#include "commands/NickCommand.h"

#include <iostream>
#include <string>
#include <vector>

namespace Irc {

	class ACommand;
	class CommandFactory {
		public:
			virtual ~CommandFactory();

			static ACommand* create(std::string& command_name, std::vector<std::string> params);

		private:
			CommandFactory();
	};
}

#endif