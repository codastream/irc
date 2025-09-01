#include "Server.h"

int main()
{
	Irc::Server* s = Irc::Server::getInstance();
	s->start();
}