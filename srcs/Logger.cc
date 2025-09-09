#include "Logger.h"


/************************************************************
*		🥚 CONSTRUCTORS & DESTRUCTOR						*
************************************************************/

const char* Logger::LOG_FILENAME = "irc.log";

Logger::Logger(bool use_console) : current_log_level_(DEBUG), use_console_(use_console) {
	log_file_.open(LOG_FILENAME, std::ios::out | std::ios::app);
	if (!log_file_.is_open())
		std::cerr << "Error opening log file" << std::endl;
}

Logger::~Logger(void) 
{
	if (log_file_.is_open())
		log_file_.close();
}

/************************************************************
*		➕ OPERATORS											*
************************************************************/

/*************************************************************
*		🛠️ FUNCTIONS											*
*************************************************************/

void	Logger::log_(LogLevel level, const std::string& msg)
{
	if (level < current_log_level_)
		return ;

	std::string level_str = "";
	switch (level)
	{
	case DEBUG:
		level_str = std::string(BLUE) + "[DEBUG]";
		break;
	case INFO:
		level_str = std::string(YELLOW) + "[INFO]";
		break;
	case ERROR:
		level_str = std::string(RED) + "[ERROR]";
		break;
	default:
		break;
	}

	level_str.append(NC);
	
	time_t			rawtime;
	struct tm*		tm;
	char			time_str[80];
	std::string		time_format = "%Y-%m-%d %H:%M:%S";

	time (&rawtime);
	tm = localtime(&rawtime);
	strftime(time_str, sizeof(time_str), time_format.c_str(), tm);

	std::string formatted_log = std::string(time_str) + " " + level_str + " " + msg + "\n";
	if (use_console_)
	{
		if (level >= ERROR)
			std::cerr << formatted_log << std::endl;
		else
			std::cout << formatted_log << std::endl;
	}
	if (log_file_.is_open())
	{
		log_file_ << formatted_log;
		log_file_.flush();
	}
}

void	Logger::debug(const std::string& msg)
{
	Logger::get_instance_().log_(DEBUG, msg);
}
void	Logger::info(const std::string& msg)
{
	Logger::get_instance_().log_(INFO, msg);
}
void	Logger::error(const std::string& msg)
{
	Logger::get_instance_().log_(ERROR, msg);
}

void Logger::ex(const std::string& msg, Irc::ReplyCode code)
{
	Logger::get_instance_().log_(ERROR, msg);
	throw Irc::IRCException(code, msg);
}

/*************************************************************
*		👁️‍ GETTERS and SETTERS				 				*
*************************************************************/

Logger&	Logger::get_instance_()
{
	static Logger instance(true);
	return instance;
}
