#include <functional>
#include <iostream>

class Op
{
	public:
		void hello( const int& v)
		{
			std::cout << "say hello in member function with v=" << v << "\n";
		}
};

int main(int argc, char*  argv[])
{
	Op op;
//	decltype( &Op::hello) _hello = &Op::hello;
	void (Op::*_hello)( const int& v)  = &Op::hello;
	std::cout << typeid( _hello ).name()  << "\n";
	auto func = std::bind( _hello, &op, std::placeholders::_1 );
	func( 3 );

	auto func2 = std::mem_fn(&Op::hello);
	func2( &op, 4);

	std::function<void(Op&, int)> func3(&Op::hello);
	func3( std::ref(op), 5);
	return 0;
}
