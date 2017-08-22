#include <iostream>
#include <vector>
#include "piped_func.h"


int main(int argc, char* argv[])
{
	using namespace func;
	auto add = piped([](int x, int y){ return x + y; });
	auto mul = piped([](int x, int y){ return x * y; });
	5 | add(2) | mul(5) | add(1) 
	  |	piped([]( int y){
	std::cout << "final output is " << y << std::endl;})();
  	    
	std::vector<int> numbers{4,8,15,16,23,42};
	using setType = decltype(numbers);
	auto where = piped2Args(whereInList<setType>);
	auto map = piped2Args(mapToList<setType>);
	auto log = piped1Arg(logList<setType>);
	auto result = numbers | where([](int x){ return (x > 10); }) | map([](int x){ return x + 5; }) | log();

	return 0;
}
