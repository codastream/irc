#ifndef REPLYASSERT_H
#define REPLYASSERT_H

#include "reply_codes.h"

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <iomanip>

class ReplyAssert {
	public:
		ReplyAssert(const std::string& reply);
		virtual ~ReplyAssert();

		ReplyAssert&	hasCode(Irc::ReplyCode code);
		ReplyAssert&	endsWith(const std::string& trailing);
		ReplyAssert&	contains(const std::string& token);
		
	private:
		std::string					reply_;
		std::string					prefix_;
		std::string					cmd_or_code_;
		std::vector<std::string>	args_;
		std::string					trailing_;
};

#endif