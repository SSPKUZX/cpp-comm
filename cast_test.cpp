#include "cast.h"

#include <cassert>

using namespace utl;

int main(){
	int k = 9;
	assert(lexical_cast(k)=="9");

	return 0;
}
