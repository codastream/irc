#ifndef LOGGER_H
#define LOGGER_H

#include "colors.h"
#include "reply_codes.h"
#include "IRCException.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>

enum LogLevel {
	DEBUG,
	INFO,
	ERROR
};

class Logger {
	public:
		virtual ~Logger();
		Logger(const Logger& other);
		Logger& operator=(const Logger& other);
		
		static void	debug(const std::string& msg);
		static void	info(const std::string& msg);
		static void	error(const std::string& msg);
		static void ex(const std::string& msg, Irc::ReplyCode code);

	private:
		static const char*			LOG_FILENAME;
		std::ofstream				log_file_;
		LogLevel					current_log_level_;
		bool						use_console_;

		Logger(bool use_console);

		static Logger&	get_instance_();
		void			log_(LogLevel level, const std::string& msg);
};


#endif