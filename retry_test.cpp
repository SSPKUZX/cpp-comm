#include "retry.h"
#include <iostream>
#include <thread>
#include <chrono>

class randomed
{
	public:
		int operator() () const
		{
			std::cout << "trying once\n";
			return 999;
		}
};

template< class FuncType>
int pick( const FuncType& f )
{
	return f();
}


int main( int argc, char* argv[])
{
	using namespace comm;
	auto pred = [](int const port)
		{ 
			return 1==port ? true : (std::this_thread::sleep_for( std::chrono::milliseconds(1000) ), false); 
		};
	std::cout << "result=\n" << retry( make_times(2, pred),
									   retry, make_times(2, pred), 
									   retry, make_times(2, pred), 
									   retry, make_times(2, pred), 
									   retry, make_times(2, pred), 
									   retry, make_times(2, pred), 
									   pick<randomed>, randomed() ) << std::endl;
	return 0;
}	

