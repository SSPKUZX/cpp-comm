#include "thread_pool.h"

#include <iostream>

using namespace std;
using namespace thd;

int main()
{
	ThreadPool tp(3);	

	tp.Run();

	auto fut1 = tp.Schedule([]{ return 3;});
	cout << fut1.get() << '\n';

	tp.Interrupt();

	return 0;
}
