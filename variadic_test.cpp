#include "variadic.h"

#include <cassert>

int main()
{
	using namespace utl;
	using namespace std;

	// index_of
	assert((index_of<int>::value == -1));
	assert((index_of<int,int>::value == 0));
	assert((index_of<int,long,int>::value == 1));

	//one_of 
	assert(!one_of<int>::value);
	assert((one_of<int,int>::value));
	assert((one_of<int,long,int>::value));

	// has_duplicate
	assert((!has_duplicate<>::value));	
	assert((!has_duplicate<int>::value));	
	assert((has_duplicate<int,int>::value));	
	assert((has_duplicate<int,long,int>::value));	

	return 0;
}
