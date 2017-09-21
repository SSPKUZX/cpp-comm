#include "any.h"

#include <iostream>
#include <cassert>

using namespace utl;

struct type{
	type(int i) : it(i){}
	int it;

	int say() &&{return 0; }
	int say() &{ return 1; } 
	int say() const&{ return 2; } 

	type& operator=( type const&) = delete;
	type(type const& rhs) : it(rhs.it){ 
		std::cout << it << " got copied\n";	
	};

	// non-templated reference conversion is supported
	operator int&(){ return it; }
};

int say(type & i){ return i.say(); }
int say(type const& i){ return i.say(); }
//int say(type i){ i.say(); return 0; }

int sayi(int& i, type const& t){ 
	assert(&i==&(t.it));	
	return 0; 
}

int main(){
	// test case 1: does static_cast support reference_type cast
	//              ---------> yes
	type t(3);
	// non-templated reference conversion is supported
	sayi(t, t);	

	// test case 2: does Any support templated refereence_type cast 
	// ----> no when signature is "operator U()"
	// ----> yes if signature of conversion operator becomes "operator U&()"
	Any k(type(5));
//	auto&& rk = static_cast<type const&>(k);//.operator type&();
	assert( say(k) == 1); 
	k = 4;
	assert(static_cast<int&>(k)==4);
// when signature is "operator U()",  above would trigger operator type
// ( which invokes copy constructor) and type->type const& conversion
// but only conversion is triggered when signature becomes "operator U&()"
//	say(k.operator const type&()); // this explicit conversion works, but above implicit conversion does not. 


	return 0;
}
