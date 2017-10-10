#include "to.h"

#include <cassert>
#include <iostream>

enum Key{
	K,
};
// not implicit convertible to integral
enum class Key2{
	V,
	S=3,
};

std::ostream& operator<< (std::ostream& os, Key2 k){
	return os << (int)(k);
}

int main(){
	using namespace utl;
	// int to string
	assert("1"==To<std::string>(1));	
	std::cout << To<std::string>(1.0f) << '\n';
	std::cout << To<std::string>(1l) << '\n';
	std::cout << To<std::string>(1ul) << '\n';

	// string to integral&float
	assert((1==To<int>("1")));	
	assert(1==To<float>("1f"));	
	assert(1==To<long>("1l"));	
	assert(1==To<unsigned long>("1ul"));	

	// enum to int and vise versa
	// integral to enum
	assert(To<Key>(0)==Key::K);
	assert(To<Key2>(3)==Key2::S);
	// enum to integral
	assert(To<long>(Key2::V)==0);

	//implicit convert 
	assert(To<int>(Key::K)==0);
	assert(To<float>(Key::K)==0.0f);
	assert(To<bool>(true));
	assert(not To<bool>(false));
	assert("h"==To<std::string>("h"));
	assert((6.0==To<int>("6f")));

	// variadic to
	assert(("123hello"==To(1,2,3,"hello")));

	// is_out_streamed
	assert(To<std::string>(Key2::S)=="3");

	return 0;
}
