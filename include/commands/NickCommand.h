#ifndef NICKCOMMAND_H
#define NICKCOMMAND_H

#include "commands/ACommand.h"
#include "Server.h"

#include <string>
#include <vector>

namespace Irc {

	class NickCommand : public ACommand {
		public:
			NickCommand();
			NickCommand(std::vector<std::string> args);
			~NickCommand();
			NickCommand& operator=(const NickCommand& other);

			void execute(Server& s, ClientConnection& co);

		private:

	};

}

#endif