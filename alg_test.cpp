#include<unordered_map>
#include<map>
#include<vector>
#include<list>
#include<deque>
#include<set>
#include<unordered_set>
#include<string>
#include<iostream>
#include<cassert>

#include"alg.h"

int main(int argc, char* argv[])
{
	using namespace alg;

	// tc1: bool & operator()
	std::map<int, int> i2i= { {2,3} };
	auto _map_it = find(i2i, 2);
	assert(_map_it);
	i2i.erase( _map_it() );
	assert(not find(i2i, 2) );

	std::unordered_map<int, int> ui2i;
	assert( not find(ui2i, 2) );

	std::set<int> si2i= {2};
	auto _set_it = find(si2i, 2);
	assert(_set_it);
	// tc2: operator*
	si2i.erase( *_set_it );
	assert(not find(si2i,2));

	// tc3: operator->
	std::unordered_set<std::string> usi2i{"haha"};
	auto h_it = find(usi2i, "haha");
	assert(h_it);
	assert(std::string(h_it->c_str())=="haha");	
	
	// test for predicate of vector
	std::vector<int> vec= {2};
	auto _vec_it = find(vec, [](int const i){return i==2;});
	assert( _vec_it );
	vec.erase( _vec_it() );
	assert(not find(vec, [](int const i){return i==2;}) );

	std::deque<int> deq ={ 3 };
	auto _deq_it = find(deq, 3 );
	assert( _deq_it );
	deq.erase( _deq_it() );
	assert(not find(deq, 3) );

	// test $transform
	std::vector<int> s1{3,4,5};
	auto s1t = transform(s1,[](int s){ return s+1;});
	assert( std::equal( s1.begin(), s1.end(), s1t.begin(), [](int a, int b){ return b==a+1;}) );

	std::vector<int> s2{3,4,5};
	auto lst2 = transform<std::list>(s2,[](int s){ return s+1;});
	assert( std::equal( s2.begin(), s2.end(), lst2.begin(), [](int a, int b){ return b==a+1;}) );
	assert( typeid(lst2)== typeid(std::list<int>) ) ;

	// testing one/none_of

	//initializer_list
	assert(alg::one_of(4, {5,6,4}) );
	assert(alg::none_of(8, {5,6,4}) );

	// sequential container
	std::vector<int> c{0,9,8};
	assert(not alg::one_of(4, c));
	assert(alg::none_of(4, c));
	assert(alg::one_of(9, c));
	assert(not alg::none_of(9, c));

	//set
	std::set<int> s{0,9,8};
	assert(not alg::one_of(4, s));
	assert(alg::none_of(4, s));
	assert(alg::one_of(9, s));
	assert(not alg::none_of(9, s));

	return 0;
}
