#include "test_utils.h"

void	noPasswordShouldError(Irc::Server* server)
{
	ServerRunner runner(*server);
	runner.start();

	ASSERT_TRUE(runner.is_running());
	int fd = make_client_socket(Irc::Server::DEFAULT_PORT);
	Logger::debug(std::string("") + Utils::str(fd));
	send_line(fd, "PASS\r\n");
	Logger::debug("after send line");
	std::string	reply = recv_line(fd);
	ReplyAssert ra(reply);
	ra.hasCode(Irc::ERR_NEEDMOREPARAMS);

	runner.stop();
	ASSERT_FALSE(runner.is_running());
}

void	testPASS(Irc::Server* server)
{
	print_test_series("command PASS");
	run_test([&] { noPasswordShouldError(server); }, "no password");
}