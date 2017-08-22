#include "str_util.h"

#include <iostream>

int main()
{
	std::string str="   sdfasd  ";

	std::cout << "the original str is=["  << str << "]\n";
	std::cout << "the trimmed str is=["  << str::trim(str) << "]\n";
}
