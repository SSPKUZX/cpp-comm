#include "capacity.h"

#include <cassert>
#include <iostream>


using namespace utl;

int main(){
	KB kb(4);
	assert(capacity_cast<Byte>(kb).Count() == 4<<10);
	assert(capacity_cast<Bit>(kb).Count() == 4<<13);
	assert(capacity_cast<MB>(kb).Count() == 0);

	MB mb(3);	
	assert((mb+kb).Count() == 3076);
	assert((kb+mb).Count() == 3076);
	assert((mb-kb).Count() == 3068);
	assert((kb-mb).Count() == -3068);

	assert((kb+=mb).Count() == 3076);
	assert((kb-=mb).Count() == 4);

	assert((kb*3).Count() == 12);
	assert((kb*=3).Count() == 12);

	assert( (int)kb == 12);
	assert( (std::string)kb == "12KB" );

	return 0;
}
