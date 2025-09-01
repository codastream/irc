#ifndef MESSAGE_H
#define MESSAGE_H

#include "User.h"

#include <string>
#include <vector>

namespace Irc {
class Message {
	public:
		Message();
		Message(const std::string& raw);
		virtual ~Message();
		Message(const Message& other);
		Message& operator=(const Message& other);
	
	private:
		std::string			raw_;
		time_t				send_time_;
		const User*			sender_;
		std::vector<User *>	receivers_;
		std::string			command_name_;
};
}
#endif