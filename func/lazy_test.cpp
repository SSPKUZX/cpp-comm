#include "lazy.h"

int main(int argc, char* argv[])
{
	using namespace lazy;
	//test code
	
	auto f = [](int x, int y, int z) { return x + y - z; };
	
	//将函数调用需要的参数保存到tuple中
	
	auto params = std::make_tuple(1, 2, 3);
	
	//将保存的参数传给函数f，实现函数调用
	
	auto result = tuple_apply(f, params); //result is 0
	
	return 0;
}
