#include <future>
using namespace std;

future<int> compute_value()
{
	int res = co_await async({
		return 30;
	});
	co_return res;
}