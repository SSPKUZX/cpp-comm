#include "traits.h"
#include "demangle.h"

#include <iostream>

class type
{
	public:
		void operator()( uint32_t const index)
		{
			std::cout << " type func operator got invoked\n";	
		}
};

struct type2 
{
	void foo() { std::cout << " foo with no parameter gets called!\n"; };
	void func( int ){	std::cout << " func with int parameter gets called!\n"; };
	public:
	int operator()(type, int, int) { std::cout << "operator() name=" << __PRETTY_FUNCTION__ << std::endl; return 9;}
};

namespace utl 
{
	has_member( foo	)
	has_member( func)
}

using namespace utl;

template<typename T>
auto test_foo(T t) ->typename std::enable_if< has_member_foo<T>::value >::type
{
	t.foo();
}

template<typename T>
auto test_func(T t) ->typename std::enable_if< has_member_func<T, int>::value >::type
{
	t.func(0);
}

template<typename T>
auto test_func_op(T t) ->typename std::enable_if< std::is_same<typename function_traits<T>::template args<0>::type,uint32_t>::value >::type
{
	std::cout << "	specified call is foo\n";
	t(1);
}


int func2(int, type){return 0;};
struct type3{};
void func3( type3 ){}
struct type4{};

inline void assert(bool yes){
	if(not yes){
		std::cout << "wrong statement \n";	
	}
}

int main(int argc, char* argv[] )
{
	// testing has_member 
	test_func_op( type() );
	test_foo( type2() );
	test_func( type2() );
	std::cout << "does class type has member function foo= " << has_member_foo<type>::value << std::endl;

	assert(is_function<type>::value);
	assert(is_signature<type2, int(type,int,int)>::value);
	// testing function_traits::arity
	// function
	assert(2==function_traits< decltype(func2)>::arity);
	// function pointer
	assert(2==function_traits< decltype(&func2)>::arity);
	// member function pointer
	assert(2==function_traits< decltype(&type2::func)>::arity);
	// function object
	assert(3==function_traits< type2 >::arity);
		
	// testing args
	// function object
	assert(std::is_same<typename function_traits<type2>::template args<0>::type,type>::value);
	// function
	assert(std::is_same<typename function_traits<decltype(func3)>::template args<0>::type,type3>::value);
	// function pointer
	assert(std::is_same<typename function_traits<decltype(&func3)>::template args<0>::type, type3>::value);
	auto lambda = [](type) { std::cout << __PRETTY_FUNCTION__ << std::endl; return 0; };
	lambda( type() );
	// lambda
	assert(std::is_same<typename function_traits<decltype(lambda)>::template args<0>::type, type>::value);
	// std::function
	assert(std::is_same<typename function_traits<std::function<void(type)>>::template args<0>::type, type>::value);

	using namespace std::placeholders;
	// tesing std::bind
	auto binded = std::bind(func2, 3, _1);
	std::cout << comm::demangle(typeid(binded)) << '\n';
	assert(is_signature<decltype(binded),int(type)>::value);
	assert(not is_signature<decltype(binded), int(std::string)>::value);
	assert(not is_invokable<decltype(binded)(std::string)>::value);

	// testing value/arity/args
	assert(function_traits<decltype(binded)>::value);
	assert(function_traits<decltype(binded)>::arity==1);
	using arg1=typename function_traits<decltype(binded)>::template args<0>::type;
	std::cout << comm::demangle<arg1>() << '\n';	
	assert(std::is_same<arg1,type>::value);
	auto binded2 = std::bind(&type2::func,_1,_2);
	using arg2=typename function_traits<decltype(binded2)>::template args<1>::type;
	assert(std::is_same<arg2,int>::value);

	return 0;
}
