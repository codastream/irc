#include "test.h"
#include "ServerRunner.h"
#include "colors.h"

int make_client_socket(int port)
{
	std::string port_str = std::to_string(port);
	struct addrinfo hints = {};
	struct addrinfo* result = nullptr;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo("localhost", port_str.c_str(), &hints, &result);
	if (status != 0)
		return -1;

	int fd = -1;

	for (struct addrinfo* test = result; test != nullptr; test = test->ai_next)
	{
		fd = socket(test->ai_family, test->ai_socktype, test->ai_protocol);
		if (fd == -1) continue;

		if (connect(fd, test->ai_addr, test->ai_addrlen) == 0)
			break;
		close(fd);
		fd = -1;
	}

	freeaddrinfo(result);
	return fd;
}

bool	send_line(int fd, const std::string& msg)
{
	return send(fd, msg.c_str(), msg.size(), 0) >= 0;
}

std::string recv_line(int fd)
{
	char	buffer[512];
	int		n = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (n <= 0)
		return {};
	buffer[n] = '\0';
	return std::string(buffer);
}

void	print_file(const std::string& msg)
{
	std::cout << BLUEBACK << msg << NC << std::endl;
}

void	print_success(const std::string& test_name)
{
	(void) test_name;
	std::cout << " ✅ "; 
}

void	print_error(const std::string& test_name)
{
	std::cout << " ❌ [" << test_name << "] ";
}


int main()
{
	Irc::Server* server;
	try
	{
		server = Irc::Server::get_instance(6667, Irc::simple_hash("password"));
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}


}