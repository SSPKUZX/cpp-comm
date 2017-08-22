#include "traits_ext.h"
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

namespace comm
{
	has_member( foo	)
	has_member( func)
}

using namespace comm;

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
auto test_func_op(T t) ->typename std::enable_if< is_functional<T, uint32_t>::value >::type
{
	std::cout << "	specified call is foo\n";
	t(1);
}

int func2(int, int){return 0;};

struct type3{};
struct type4{};
void func3( type3 ){}

int main(int argc, char* argv[] )
{
	test_func_op( type() );
	test_foo( type2() );
	test_func( type2() );

	std::cout << "does class type has member function foo= " << has_member_foo<type>::value << std::endl;

	std::cout << "get_type<1, int, double>=" << comm::demangle(typeid(typename comm::get_type<1, int, double>::type)) << "\n";
//	std::cout << "get_type<2, int, double>=" << comm::demangle(typeid(typename comm::get_type<2, int, double>::type)) << "\n";

	std::cout << "argument count of func2= " << argument_count< decltype(func2)>::value << std::endl;
	std::cout << "argument count of type2.func= " << argument_count< decltype(&type2::func)>::value << std::endl;
	std::cout << "argument count of function class type2=" << argument_count< type2 >::value << std::endl;
		
	auto f1= std::bind( type2(), type(), 0, 0 );
	std::cout << "first_arg of type2="  << typeid( typename func_trait<type2>::first_arg).name() << std::endl;
	std::cout << "first_arg of func3="  << typeid( typename func_trait<decltype(func3)>::first_arg).name() << std::endl;
	auto lambda = [](type const&) { std::cout << __PRETTY_FUNCTION__ << std::endl; return 0; };
	lambda( type() );
	std::cout << "typeinfo of lambda="  << comm::demangle(typeid(decltype(lambda))) << std::endl;
//	std::cout << "first_arg of lambda="  << typeid( typename func_trait<decltype(lambda)>::first_arg).name() << std::endl;
	return 0;
}
