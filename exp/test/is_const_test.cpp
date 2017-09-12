#include <iostream>

template<class T>
struct is_const : public std::integral_constant<bool, 
				std::is_const<typename std::remove_reference<T>::type>::value  || 
				std::is_const<typename std::remove_extent<T>::type>::value  || 
				std::is_const<typename std::remove_pointer<T>::type>::value >
	{};

int main(){
	std::cout << std::boolalpha << is_const<int const&>::value << '\n';
	std::cout << std::boolalpha << is_const<int const[]>::value << '\n';
	std::cout << std::boolalpha << is_const<int const * const>::value << '\n';
	std::cout << std::boolalpha << is_const<int const>::value << '\n';

	return 0;
}
