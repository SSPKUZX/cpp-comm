#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <cassert>

#include "pipelize.h"

int plus2(int a, int b){ return a+b;}

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


  	    
//	std::vector<int> numbers{4,8,15,16,23,42};
//	std::deque<int> numbers{4,8,15,16,23,42};
	std::list<int> numbers{4,8,15,16,23,42};
//	using setType = decltype(numbers);
//	auto where = piped2Args(whereInList<setType>);
//	auto map = piped2Args(mapToList<setType>);
//	auto log = piped1Arg(logList<setType>);
	auto const& cn1 = numbers;
	auto&&  cn2 = cn1 | filter([](int x){ return (x > 10); });
	assert(&cn1!=&cn2);
	auto&&  n2 = numbers | filter([](int x){ return (x > 10); }) | filter(std::less<int>(), _1, 25);
	assert(&numbers==&n2);

	n2 | map([](int x){ return x + 1; }) | map<std::vector>( plus2, _1,2) | map<std::deque>( std::plus<int>(), _1,3) | log();

	return 0;
}
