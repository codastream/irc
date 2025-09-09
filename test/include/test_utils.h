#ifndef TEST_H
#define TEST_H

#include "test_list.h"

#include <iostream>
#include <sstream>
#include <string>
#include <source_location>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>			// memset

#define ASSERT_FALSE(cond) if (cond) throw std::runtime_error("Fail: should not " #cond);
#define ASSERT_TRUE(cond) if (!(cond)) throw std::runtime_error("Fail: " #cond);
#define ASSERT_EQ(a, b) if (!((a) == (b))) { \
	std::ostringstream oss;\
	oss << "Fail: " << #a << " == " << #b;\
	throw std::runtime_error(oss.str()); \
}

template <typename Exception, typename Func>
bool assert_throws(Func&& f);

template <typename Func>
void run_test(Func&& f, const char* name);

#define RUN_TEST(f) run_test(f, __func__)

int			make_client_socket(int port);
bool		send_line(int fd, const std::string& msg);
std::string	recv_line(int fd);

void		print_test_series(const std::string& msg);
void		print_success(const std::string& test_name);
void		print_error(const std::string& test_name);

// to test console output
struct CoutRedirect {
	std::stringstream	buffer;
	std::streambuf* 	old;

	CoutRedirect() : old(std::cout.rdbuf(buffer.rdbuf())) {}
	~CoutRedirect() {std::cout.rdbuf(old);}

	std::string str() const { return buffer.str(); }
};

#include "test_utils.tpp"

#endif