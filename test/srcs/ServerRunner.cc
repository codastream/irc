#include "ServerRunner.h"

/************************************************************
*		🥚 CONSTRUCTORS & DESTRUCTOR						*
************************************************************/

ServerRunner::ServerRunner(Irc::Server& server) : server_(server), running_(false) {}

ServerRunner::~ServerRunner(void) 
{
	if (thread_.joinable())
	{
		server_.stop();
		thread_.join();
	}
	stop();
}

/*************************************************************
*		🛠️ FUNCTIONS											*
*************************************************************/

void		ServerRunner::start()
{
	if (running_) 
		throw std::runtime_error("Server already running");
	running_ = true;
	thread_ = std::thread([this] () {
		try {
			server_.loop();
		} catch (const std::exception& e)
		{
			last_error_ = e.what();
		}
		running_ = false;
	});
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void		ServerRunner::stop()
{
	if (!running_)
		return;
	server_.stop();
	if (thread_.joinable())
		thread_.join();
	running_ = false;
}

/*************************************************************
*		👁️‍ GETTERS and SETTERS				 				*
*************************************************************/

bool		ServerRunner::is_running() const
{
	return running_;
}

std::string	ServerRunner::last_error() const
{
	return last_error_;
}