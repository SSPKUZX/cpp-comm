#include "simple_retry.h"

#include <iostream>
#include <cassert>

int main()
{
	auto is_valid = [](int ret){return ret > 2;};
	uint32_t init = 0;
	auto work	= [&init]{ return ++init; };
	auto ret	= cmn::retry( 2, is_valid, work); 
	assert( ret == 2);
	std::cout << ret << '\n';

	init	= 0;
	ret		= cmn::retry( 3, is_valid, work); 
	assert( ret == 3);

	std::cout << ret << '\n';

	return 0;
}
