#include "Message.h"

namespace Irc {
/************************************************************
*		🥚 CONSTRUCTORS & DESTRUCTOR						*
************************************************************/

Message::Message() : raw_(""), send_time_(0), sender_(NULL), \
	receivers_(), command_name_("") {}

Message::Message(const std::string& raw) : raw_(raw), send_time_(0), sender_(NULL), \
	receivers_(), command_name_("") {}

Message::Message(const Message& inst) : raw_(inst.raw_), send_time_(inst.send_time_), sender_(inst.sender_), \
	receivers_(inst.receivers_), command_name_(inst.command_name_) {}

Message::~Message(void) {}

/************************************************************
*		➕ OPERATORS											*
************************************************************/

Message& Message::operator=(const Message& inst) 
{
	if (this != &inst)
	{
		raw_ = inst.raw_;
		send_time_ = inst.send_time_;
		sender_ = inst.sender_;
		receivers_ = inst.receivers_;
		command_name_ = inst.command_name_;
	}
	return (*this);
}

/*************************************************************
*		🛠️ FUNCTIONS											*
*************************************************************/

/*************************************************************
*		👁️‍ GETTERS and SETTERS				 				*
*************************************************************/
}