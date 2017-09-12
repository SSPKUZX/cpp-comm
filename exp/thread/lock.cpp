#include <mutex>
#include <thread>

int main( int argc, char* argv[])
{
	std::mutex	mutex;
	int			shared_value;

	// lock_guard
	{
		std::lock_guard< std::mutex > lg( mutex );	
		shared_value = 123;
	}

	// std::lock
	std::mutex	mutex2;
	int			shared_value2;
	{
		std::lock( mutex, mutex2 ); // lock both or neither at the same time 	
		std::lock_guard< std::mutex > lg( mutex, std::adopt_lock); // obtain the locked mutex and release it out of scope
		std::lock_guard< std::mutex > lg2( mutex2,std::adopt_lock);
		shared_value2 = shared_value;
	}


	// unique_lock
	{
		std::unique_lock<std::mutex> ul( mutex, std::defer_lock); // 1 
	    std::unique_lock<std::mutex> ul2(mutex2,std::defer_lock); // 1 std::def_lock 留下未上锁的互斥量
		std::lock( ul, ul2); // 2 互斥量在这里上锁
		std::swap( shared_value, shared_value2);
	}

	return 0;
}
