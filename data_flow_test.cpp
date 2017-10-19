#include "data_flow.h"

#include <list>
#include <deque>
#include <iostream>
#include <cassert>

using namespace utl;

int plus2(int a, int b){ return a+b;}

int main(){

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

	read_file("online_ips.txt") | sort() | differenciate( read_file("all_ips.txt") ) | log();
	return 0;
}
