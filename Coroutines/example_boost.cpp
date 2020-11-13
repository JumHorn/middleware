#include <boost/coroutine2/coroutine.hpp>
#include <iostream>
#include <vector>
using namespace std;

/*
g++ -std=c++11 example_boost.cpp -lboost_context

boost 协程push_type将数据送给pull_type
push_type ==> pull_type
*/

int main()
{
	using coro_t = boost::coroutines2::coroutine<int>;

	auto source = coro_t::pull_type([](coro_t::push_type &yield) {
		for (auto i = 0; i != 3; ++i)
		{
			yield(i);
		}
	});

	for (auto value : source)
	{
		std::cout << value << std::endl;
	}
	return 0;
}