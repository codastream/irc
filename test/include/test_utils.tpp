#include "test_utils.h"

template <typename Exception, typename Func>
bool assert_throws(Func&& f)
{
	try {
		f();
	} catch (const Exception&) {
		return true;
	} catch (...) {
		return false;
	}
	return false;
}

template <typename Func>
void run_test(Func&& f, const char* name)
{
	try {
		f();
		print_success(name);
	} catch (const std::exception& e) {
		print_error(name);
	}
}