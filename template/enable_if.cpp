#include <iostream>
//#include "type_traits.h"

template<bool B, typename T = void >
struct enable_if{};

template<typename T>
struct enable_if<true, T>
{
	typedef T type;
};

template<typename T, 
		 typename enable_if<std::is_integral<T>::value, int>::type n = 0>
void what_am_i(T x)
{
	std::cout << x << " is integral number" << std::endl;
}
		 
template<typename T>
auto what_am_i(T x) -> typename enable_if<std::is_pointer<T>::value, void>::type 
{
		std::cout << x << " is a pointer" << std::endl;
}

template<typename T,
		 typename enable_if<std::is_floating_point<T>::value, int>::type n = 0> 
void what_am_i(T x)
{
	std::cout << x <<" is float number" << std::endl;
}

int main()
{
	what_am_i(123);
	what_am_i((float)123.2);
    int *p = new int(3);
	what_am_i(p);

	return 0;
}
