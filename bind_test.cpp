#include "bind.h"
#include "demangle.h"

#include <iostream>

/*
 * implement a completely customized bind
 * in which user can call bind(f,a,_1)(b) just the same as f(a,b)
 *  step one : assume placeholders are placed in bind in order like _1,_2...
 *				--> DONE
 *  step two : bind value to corresponding placeholders even placeholders are in disorder
 *
 * */

using namespace utl;

struct Dumy{
	int say(int const&){ return 9;}
	char operator()(int){ return 'k';}
};

int func(int i,long const& l,char const& c){ 
	std::cout << i << '\t' << l << '\t' << c << '\n'; 	
	return 3;
}

int main(){
	auto f2 = Bind(func,2,_1,'2');
	std::cout << demangle(f2) << '\n';	
	std::cout << f2(3l) << '\n';

	auto f3 = Bind(func,2,_1,_2);
	std::cout << demangle(f3) << '\n';	
	std::cout << f3(3l,'c') << '\n';	
	std::cout << demangle(&decltype(f3)::operator()) << '\n';

	Dumy d;
	auto f4 = Bind(&Dumy::say, &d, _1);
	std::cout << f4(4) << '\n';
	std::cout << demangle(f4) << '\n';	

	Dumy d2;
	auto f5 = Bind2(&Dumy::say, &d2, _1);
	std::cout << f5(4) << '\n';
	std::cout << demangle(f5) << '\n';	

	auto f7 = Bind(Dumy(),  _1);
	std::cout << f7(4) << '\n';
	std::cout << demangle(f7) << '\n';	

	std::cout << demangle(std::mem_fn(&Dumy::say)) << '\n';

	std::cout << demangle<decltype(PHVsGenerator<decltype(_2),int,decltype(_1)>()())>() << '\n';

	// test placeholders in disorder
	auto f8 = Bind(func,_2,_1,_3);
	std::cout << demangle(f8) << '\n';
	f8(1l,3,'c');

	auto f9 = Bind(func,_1,_1,_3);
	std::cout << demangle(f9) << '\n';
	f9(1l,3,'c');

	return 0;
}
