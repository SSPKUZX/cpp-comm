#include "atomic_struct.h"

#include <cassert>

struct Key{
	char a;
	char b;
	char c;
	char d;
};

int main(){
	using namespace utl;

	AtomicStruct<Key> ak;
	assert(sizeof(ak)==4);

	return 0;
}
