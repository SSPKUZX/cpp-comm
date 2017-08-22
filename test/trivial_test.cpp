#include <iostream>
#include <type_traits>

struct stt
{
	long _long;
	char _char;
	int* _p;
};

class cls
{
	public:
		cls() : k(0), p(nullptr){}
	//	virtual vlid virt() = 0;
	private:
		void y(){}
		int		k;
		int*	p;
};

/**
 * 1>for those trivial types, we could use malloc,free,memset,memcpy
 *		all defaults(constructor,desctructor,operator,copy ctor) are default
 * 2>for those standard_layout, we could precisely predict the address
 *	of each field
 *		requirement: 1) all fields have the same access privileage
 *			2) only one class has non-static fields in whole hierarchy tree
 *			3) the first field in derived class can't be its base class
 *			4) no virtual base class or function
 * 3>for those pod types, no need to call constructor and destructor(in STL)
 *		it supports static initialization and c-language struct memory layout
 *
 * */
int main()
{
	std::cout << std::boolalpha <<  "is array[] trivial:" << std::is_trivial<int[]>::value << '\n';
	std::cout << std::boolalpha <<  "is array[N] trivial:" << std::is_trivial<int[3]>::value << '\n';

	std::cout << std::boolalpha <<  "is string trivial:" << std::is_trivial<std::string>::value << '\n';
	std::cout << std::boolalpha <<  "is struct trivial:" << std::is_trivial<stt>::value << '\n';
	std::cout << std::boolalpha <<  "is class trivial:" << std::is_trivial<cls>::value << '\n';
	std::cout << std::boolalpha <<  "is class standard layout:" << std::is_standard_layout<cls>::value << '\n';
	std::cout << std::boolalpha <<  "is class a pod:" << std::is_pod<cls>::value << '\n';

	return 0;
}
