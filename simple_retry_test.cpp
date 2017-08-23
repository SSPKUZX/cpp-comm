#include "simple_retry.h"

#include <iostream>
#include <cassert>

static int x=0;
bool is_not_three(){  return ++x!=3; }

int main()
{
	auto is_valid = [](uint32_t ret){return ret > 2;};
	uint32_t init = 0;
	auto work	= [&init]{ return ++init; };
	auto ret	= cmn::retry( work, is_valid, 2); 
	assert( ret == 2);
	std::cout << ret << '\n';

	init	= 0;
	ret		= cmn::retry( work, is_valid); 
	assert( ret == 3);
	std::cout << ret << '\n';

	auto is_true = cmn::retry([&init]{ return --init==0;}, 2);	
	std::cout << std::boolalpha << is_true << '\n';

	is_true = cmn::retry([&init]{ return ++init==3;});	
	std::cout << std::boolalpha << is_true << '\n';

	is_true = cmn::retry( cmn::fnot(is_not_three) );	
	std::cout << std::boolalpha << is_true <<"\tx=" << x << '\n';

	is_true = cmn::retry( [](){ return !is_not_three(); } );	
	std::cout << std::boolalpha << is_true <<"\tx=" << x << '\n';

	if(not false){ std::cout << "<not false> is syntactically right\n";}
	if( true not_eq false ){ std::cout << "<true not_eq false> is syntactically right\n";}

	return 0;
}
