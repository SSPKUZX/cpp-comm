#include <iostream>

void __attribute__((__weak__)) func()
{
	std::cout << "weak attributed func\n";
}

int main()
{
	func();

	return 0;
}
