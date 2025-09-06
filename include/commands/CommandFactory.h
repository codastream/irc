#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

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