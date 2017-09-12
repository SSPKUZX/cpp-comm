#include <atomic>
#include <thread>
#include <mutex>
#include <iostream>

int main(int argc, char* argv[])
{
	std::atomic< int >	count(0);
	std::mutex io_mutex;
	std::thread  producer( [&count]() { count.store(100, std::memory_order_release); } );
	auto _consume = [&count, &io_mutex]() 
				{ 
					int _index;
					while( (_index = count.fetch_sub(1, std::memory_order_acquire) ) <= 0 ); 
					while( _index > 0)
					{
						{
							std::lock_guard<std::mutex> lg(io_mutex);
							std::cout << "got index=" << _index << "\t" << std::this_thread::get_id() << std::endl;
						}
						_index = count.fetch_sub(1, std::memory_order_relaxed);
						_index = count.fetch_sub(1, std::memory_order_relaxed);
						std::this_thread::yield();
						_index = count.fetch_sub(1, std::memory_order_relaxed);
						{
							std::lock_guard<std::mutex> lg(io_mutex);
							std::cout << "relaxed got index=" << _index << "\t" << std::this_thread::get_id() << std::endl;
						}
						_index = count.fetch_sub(1, std::memory_order_acquire);
					}
				};
	std::thread  consumer1(_consume); 
	std::thread  consumer2(_consume); 
//	std::thread  consumer3(_consume); 
	producer.join();
	consumer1.join();
	consumer2.join();
//	consumer3.join();

	// analysis : just from the result, even fetch_sub gets a memory_order_relaxed, it store and load will still be synchronized
	// from another perspective, a relaxed fetch_sub is composed of a relaxed load and a relaxed store, because they are in the same batch with the last fetch_sub's acquire load, they're synced 
	return 0;
}
