#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <cassert>

#include "pipelize.h"


int main(int argc, char* argv[])
{
	using namespace utl;
	auto add = pipelize([](int x, int y){ return x + y; });
	auto mul = pipelize([](int x, int y){ return x * y; });
	5 | add(2) | mul(5) | add(1) 
	  |	pipelize([]( int y){
	std::cout << "final output is " << y << std::endl;})();

	3 | [](int x){ std::cout<< x << '\n'; return std::string("piped string"); }
	  | [](std::string const& msg) { std::cout << "message is=" << msg << '\n';};	


  	    

	return 0;
}
