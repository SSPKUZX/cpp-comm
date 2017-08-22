#include <future>
#include <iostream>

/*
 * The final future referring to a shared state for a non-deferred task launched via
 * std::async blocks until the task completes
 *
 * valid future only comes from three ways: promise, packaged_task, async
 * */


int func()
{
	return 109;
}

std::future<void> packaged_future()
{
	std::packaged_task<void()> pt(func);
	auto fut = pt.get_future();
	std::thread bg(pt); 
	bg.detach();
	return fut;
}

std::future<void> promised_future()
{
	std::promise<int> pt(func);
	auto fut = pt.get_future();
	std::thread bg(pt); 
	bg.detach();
	return fut;
}
int main( int argc, char* argv[])
{
	// use examples of std::future
	auto fut_1	= std::async( func );
	auto fut_2	= std::async( std::launch::async | std::launch::deferred, func);	

	std::cout << "default future:"  << fut_1.get() << std::endl;
	std::cout << "customed future:"  << fut_2.get() << std::endl;

	// use examples of std::packaged_task
	std::cout << "packaged_task generated future:"  << waitResult().get() << std::endl;

	// use examples of std::promise
	std::cout << "promise generated future:"  << waitResult().get() << std::endl;
	
		

	return 0;
}


