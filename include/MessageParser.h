#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include "ACommand.h"
#include "CommandFactory.h"

#include <string>

class MessageParser {
	public:
		MessageParser();
		virtual ~MessageParser();
		MessageParser(const MessageParser& other);
		MessageParser& operator=(const MessageParser& other);

		ACommand& parseCommand(const std::string& msg);
		
	private:

};

#endif