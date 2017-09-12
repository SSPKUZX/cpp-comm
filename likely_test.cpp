#include "Likely.h"
#include <iostream>

struct st
{};

int main( int argc, char* argv[])
{
//	int* p = nullptr;
//	if( LIKELY(p) )
//		std::cout << "p is not null\n";
//	else
//		std::cout << "p is null\n";
	
	int k = 0;
	if( LIKELY(k) )
		std::cout << "k is not null\n";
	else
		std::cout << "k is null\n";

//	st s;
//	if( LIKELY(s) )
//		std::cout << "s is not null\n";
//	else
//		std::cout << "s is null\n";


	return 0;
}
