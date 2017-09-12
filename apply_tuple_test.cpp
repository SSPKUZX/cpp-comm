#include "apply_tuple.h"
#include <iostream>


int func(int v1, double v2)
{ return 0;}

int main(int argc, char* argv[])
{
	using namespace utl;		
	std::cout << "applyTuple result: " << ApplyTuple( func, std::make_tuple(1, 9.0) ) << std::endl;
	return 0;
}
