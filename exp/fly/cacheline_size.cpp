#include "cacheline_size.h"

#include <cassert>

int main(){
	assert( 64 == cacheline_size());

	return 0;
}
