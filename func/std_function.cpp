#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>

using namespace std;

int func(int a, int b)
{
	std::cout << "hello\n";
	return 0;
}

struct type
{
	void say(int, int){}
};

int main()
{
	vector<int> v;
	for(int i= 0; i < 10; ++i)
		v.push_back(i);
	int x = 6;

	vector<int>::iterator it = find_if(v.begin(), v.end(), bind2nd(greater<int>(), x));
	if(it != v.end())
		std::cout << "the first greater number got found :" << *it << std::endl;

	// function object can only be used to init callable without argument bindings
	std::function< int(int, int) > raw_func(func);
	raw_func(1, 2);

	std::function< int(int) > bind_func( std::bind( func, 1, std::placeholders::_1) );
	bind_func( 2 );

	type t;
	// 
	std::function<void(int,int)> t_f = std::bind(&type::say, &t, std::placeholders::_1, std::placeholders::_2);
	t_f(1,2);

	return 0;
}
