#include <iostream>
#include <vector>
#include "piped_func.h"

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


  	    
	std::vector<int> numbers{4,8,15,16,23,42};
//	using setType = decltype(numbers);
//	auto where = piped2Args(whereInList<setType>);
//	auto map = piped2Args(mapToList<setType>);
//	auto log = piped1Arg(logList<setType>);
	numbers | where([](int x){ return (x > 10); }) | map([](int x){ return x + 5; }) | log();

	// how can we achieve " numbers | where([](int x){x>0;}) | log(); " 
	// without explicitly specifiy types through piped2Args 

	return 0;
}
