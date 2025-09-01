#include "Server.h"

int main()
{
	Irc::Server* s = Irc::Server::get_instance();
	signal(SIGINT, Irc::Server::handle_interrupt);
	try
	{
		s->start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	epoll_event*	events = s->get_events();
	while (s->can_serve())
	{
		int n = epoll_wait(s->get_epoll_fd(), events, Irc::Server::QUEUE_SIZE, -1);
		if (n == -1)
		{
			s->stop();
		}
		for (int i = 0; i < n; ++i)
		{
			if (events[i].data.fd == s->get_server_fd())
			{
				int client_fd = accept(s->get_server_fd(), NULL, NULL);
				Irc::Server::set_non_blocking(client_fd);
				s->subscribe_to_event(client_fd, EPOLLIN | EPOLLET);
			}
			else
			{
				char readBuf[512];
				errno = 0;
				ssize_t len = recv(events[i].data.fd, readBuf, sizeof(readBuf), 0);
				if (len == -1)
				{
					std::cerr << "rcv error" << std::endl;
					close(events[i].data.fd);
				}
				else if (len == 0)
				{
					std::cout << "EOF" << std::endl;
					close(events[i].data.fd);
				}
				else
				{
					readBuf[len] = '\0';
					std::cout << "msg: " << readBuf << std::endl;
				}
			}
		}
	}

}