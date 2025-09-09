#ifndef SERVERRUNNER_H
#define SERVERRUNNER_H

#include "Server.h"

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <stdexcept>
#include <chrono>

class ServerRunner {
	public:
		ServerRunner(Irc::Server& server);
		virtual ~ServerRunner();

		void		start();
		void		stop();
		bool		is_running() const;
		std::string	last_error() const;
		
	private:
		Irc::Server& server_;
		std::thread thread_;
		std::atomic<bool> running_;
		std::string last_error_;

};

#endif