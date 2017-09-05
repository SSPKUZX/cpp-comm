#include<unordered_map>
#include<map>
#include<vector>
#include<list>
#include<deque>
#include<set>
#include<unordered_set>
#include<iostream>

#include"stl.h"

int main(int argc, char* argv[])
{
	using namespace comm;

	std::map<int, int> i2i= { {2,3} };
	auto _map_it = find(i2i, 2);
	if( _map_it )
	{
		std::cout << "find 2, erase it\n";
		i2i.erase( _map_it.get() );
	}
	else
		std::cout << "not find 2\n";

	std::unordered_map<int, int> ui2i;
	if( find(ui2i, 2) )
		std::cout << "find 2\n";
	else
		std::cout << "not find 2\n";

	std::set<int> si2i= {2};
	auto _set_it = find(si2i, 2);
	if( _set_it )
	{
		std::cout << "find 2, erase it\n";
		si2i.erase( _set_it.get() );
	}
	else
		std::cout << "not find 2\n";

	std::unordered_set<int> usi2i;
	if( find(usi2i, 2) )
		std::cout << "find 2\n";
	else
		std::cout << "not find 2\n";

	// test for predicate of vector
	std::vector<int> vec= {2};
	auto _vec_it = find(vec, [](int const i){return i==2;});
	if( _vec_it )
	{
		std::cout << "find 2, erase it\n";
		vec.erase( _vec_it.get() );
	}
	else
		std::cout << "not find 2\n\n";

	std::deque<int> deq ={ 3 };
	auto _deq_it = find(deq, 3 );
	if( _deq_it )
	{
		std::cout << "find 3, erase it\n";
		deq.erase( _deq_it.get() );
	}
	else
		std::cout << "not find 3\n\n";


	// test $transform
	std::vector<int> s1{3,4,5};
	for(auto& i : transform(s1,[](int s){ return s+1;})){
		std::cout << "\t" << i;	
	}
	std::cout << '\n';

	std::vector<int> s2{3,4,5};
	auto lst2 = transform<std::list>(s2,[](int s){ return s+1;});
	for(auto& i : lst2){
		std::cout << "\t" << i;	
	}
	std::cout << '\n';
	std::cout << typeid(lst2).name() << '\n';

	return 0;
}
