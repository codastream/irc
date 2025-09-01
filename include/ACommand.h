#ifndef COMMAND_H
#define COMMAND_H

#include "Server.h"
#include "Client.h"
#include "Message.h"

#include <string>

namespace Irc {
class ACommand {
	public:
		ACommand();
		virtual ~ACommand();
		ACommand(const ACommand& other);
		ACommand& operator=(const ACommand& other);
		
		virtual void execute(Server& s, Irc::Client& c, Irc::Message& m) = 0;

	private:

};
}

#endif