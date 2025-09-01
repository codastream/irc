#ifndef PASSCOMMAND_H
#define PASSCOMMAND_H

#include "ACommand.h"

#include <iostream>
#include <string>

namespace Irc {
class PassCommand : public ACommand {
	public:
		PassCommand();
		virtual ~PassCommand();
		PassCommand& operator=(const PassCommand& other);

		void execute(Server& s, Client& c, Message& m);

	private:

};
}

#endif