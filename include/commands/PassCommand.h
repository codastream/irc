#ifndef PASSCOMMAND_H
#define PASSCOMMAND_H

#include "ACommand.h"
#include "Server.h"

#include <string>
#include <vector>
#include <stdexcept>

namespace Irc {

	class PassCommand : public ACommand {
		public:
			PassCommand();
			PassCommand(std::vector<std::string> args);
			~PassCommand();
			PassCommand& operator=(const PassCommand& other);

			void execute(Server& s, ClientConnection& co);

		private:

	};

}

#endif