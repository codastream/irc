#include "test_utils.h"

void	noPasswordShouldError(Irc::Server* server)
{
	ServerRunner runner(*server);
	runner.start();

	ASSERT_TRUE(runner.is_running());
	int fd = make_client_socket(6667);

	send_line(fd, "PASS\r\n");

	std::string	reply = recv_line(fd);
	ReplyAssert ra(reply);
	ra.hasCode(Irc::ERR_NEEDMOREPARAMS);

	runner.stop();
	ASSERT_FALSE(runner.is_running());
}

void	testPASS(Irc::Server* server)
{
	RUN_TEST([&] { noPasswordShouldError(server); });
}