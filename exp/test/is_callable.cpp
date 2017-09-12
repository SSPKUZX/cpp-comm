#include <iostream>

namespace cmn
{
	template<class F, class... Args>
	struct is_callable
	{
		template<typename T, typename = typename std::result_of<T(Args...)>::type>
		static constexpr std::true_type
		check(std::nullptr_t) { return std::true_type{}; };

		template<typename T>
		static constexpr std::false_type
		check(...) { return std::false_type{}; };

		static constexpr bool value = decltype(check<F>(nullptr))::value;
	};
}

void k(int) {}

using k_ref = int(&)(int);
using k_ptr = int(*)(int);

struct Functor
{
	void operator() (int){}
};

int main()
{
	// return true
	std::cout << std::boolalpha << cmn::is_callable<decltype(&k), int>::value << '\n';	
	// return false b' function should be represented as pointer
	std::cout << std::boolalpha << cmn::is_callable<decltype(k), int>::value << '\n';	

	// return true
	std::cout << std::boolalpha << cmn::is_callable<k_ref, int>::value << '\n';	
	std::cout << std::boolalpha << cmn::is_callable<k_ptr, int>::value << '\n';	

	// return true
	std::cout << std::boolalpha << cmn::is_callable<Functor, int>::value << '\n';	

	return 0;
}

