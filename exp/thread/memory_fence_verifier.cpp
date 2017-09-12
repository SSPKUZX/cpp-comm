#include <atomic>
#include <thread>
#include <iostream>

int main(int argc, char* argv[])
{
	std::atomic<bool>	x(false),y(false);  
	// if x is just an ordinary integer, it works here too
	std::atomic<int>	z(0);

	std::thread x_y( [&]()
					{
						x.store(true, std::memory_order_relaxed);
						std::atomic_thread_fence( std::memory_order_release );
						y.store(true, std::memory_order_relaxed);	
					});
	std::thread y_x( [&]()
					{
						while( !y.load( std::memory_order_relaxed) );		
						std::atomic_thread_fence( std::memory_order_acquire );
						if( x.load( std::memory_order_relaxed) )
							++z;
					});	
	x_y.join();
	y_x.join();		
	std::cout << "the final z is " << z.load() << std::endl;
}
