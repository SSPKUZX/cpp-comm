#include "tuple.h"

#include <iostream>

int main(){
	using namespace utl;

	auto tup = std::make_tuple(3,"string", 4l);
	std::cout << tup  << '\n';

	return 0;
}
