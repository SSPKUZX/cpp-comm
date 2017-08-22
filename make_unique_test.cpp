#include "make_unique.h"

#include <iostream>

using namespace cmn;

int main()
{
	auto x = make_unique<int>(8);
	std::cout << "x=" << *x << '\n';

	auto arr = make_unique<int[]>(5);
	arr[0] = 4;
	std::cout << "arr[0]=" << arr[0] << '\n';
}
