#include <iostream>
#include "demangle.h"

int f(int, int) {return 0;}
namespace mrv2
{
	struct Hello{};
}

int main()
{
	using namespace utl;
	std::cout << "demangled int is=" << demangle( typeid(int) ) << "\n";
	std::cout << "demangled fuc is=" << demangle( typeid(f) ) << "\n";
	std::cout << "demangled fuc is=" << demangle( typeid(mrv2::Hello) ) << "\n";
	return 0;
}
