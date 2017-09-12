#define NDEBUG // disable assertion

#include <iostream>
#include <cassert>


void test( int* int_ptr){
	assert( (true, nullptr != int_ptr));
	if( int_ptr )
		std::cout << *int_ptr << '\n';
	else
		std::cout << "nullptr parameter\n";
}

int main(){
	int* iptr = new int(3); 
	int* nptr = nullptr;
	test(iptr);
	test(nptr);

	std::cout << std::boolalpha << (bool)__builtin_expect(0,0) << true <<'\n';
	return 0;
}
