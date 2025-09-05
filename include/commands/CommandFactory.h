#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include "ACommand.h"
#include "PassCommand.h"
#include "NickCommand.h"

#include <iostream>
#include <string>

namespace Irc {

class CommandFactory {
	public:
		virtual ~CommandFactory();

		static ACommand* create(std::string& command_name);

	private:
		CommandFactory();
};
}

#endif