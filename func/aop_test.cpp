#include "aop.h"
#include <iostream>
#include <thread>

int snap()
{
	std::this_thread::sleep_for( std::chrono::milliseconds(100) );
	return 100;
}

int main(int argc, char* argv[])
{
	using namespace aop;	
	logged( "ravine", std::function<int()>(snap) )();

	return 0;
}
