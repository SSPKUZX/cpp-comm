#include "cast.h"

#include <cassert>

using namespace utl;

int main(){
	int k = 9;
	assert(lexical_cast(k)=="9");

	int  i =3;
	//T&&
	assert((std::is_same<int, decltype(unconst(std::move(i)))>::value));	
	//T&
	assert((std::is_same<int&, decltype(unconst(i))>::value));	
	const int  ci =3;
	//const T&&
	assert((std::is_same<int, decltype(unconst(std::move(ci)))>::value));	
	//const T&
	assert((std::is_same<int&, decltype(unconst(ci))>::value));	
	//T
	assert((std::is_same<int, decltype(unconst(3))>::value));	
	//const T
	assert((std::is_same<int, decltype(unconst((const int)(3)))>::value));	

	return 0;
}
