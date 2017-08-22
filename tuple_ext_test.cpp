#include "tuple_ext.h"
#include <iostream>

int i2ii(int, int){ return 999; }

int main(int argc, char* argv[])
{
	using namespace comm;	
	
    auto ret = call_through_tuple( i2ii, 1, 1);
	std::cout << "expect to be 999, actual is=" << ret << std::endl; 
}
