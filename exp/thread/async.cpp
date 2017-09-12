/*
 * prefer task-based to thread-based asynchronization
 * ie, use std::async over std::thread to do things asyncly
 * */

#include <future>
#include <iostream>

void doAsyncWork()
{
	std::this_thread::sleep_for( std::chrono::milliseconds(1000) );
	std::cout << "finished async work\n";
}

int main()
{
	// using std::async in the default way won't guarantee a new thread
	// will be created, but may run on current thread in appropriate time
	auto fut_default = std::async( doAsyncWork );

	// if you want to guarantee the async work running in another thread
	// use the std::launch::async policy
	// [ std::launch::deferred means async work may get executed syncly until 
	//   get or wait gets invoked on the calling thread. ]
	auto fut_async	= std::async( std::launch::async, doAsyncWork);

	// [Note that the default policy is std::launch::deferred | std::launch::async. ]
	// so the default policy may create bugs when wait_for or wait_until gets invoked  
	// on futures returned by std::async  like the following
	// [ when the default async runs in deferred policy, it may never stop because
	//  std::future_status::deferred will always be returned. to avoid this, a check
	//  t see if it's a deferred task should be done like this :
	// if( std::future_status::deferred == fut_default.wait_for( std::chrono::milliseconds(0) )
	// {  //bulabula  } else { bulalula} ]
	//
	// ALSO thread_local variables would be unclear because it may run syncly or asyncly
	while( std::future_status::ready != fut_default.wait_for( std::chrono::milliseconds(200) ) )
	{
		std::cout << "waiting future_status to be ready\n";
	}

	// time to use std::thread rather than std::async
	// 1> need low-level platform-specific API through std::thread::native_handler
	// 2> optimize thread usage for specific application with known execution 
	//    profiles and fixed hardware characteristics
	// 3> implement threading tech beyong the c++ concurrent API, like thread pool

	return 0;
}
