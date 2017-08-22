#include <iostream>
#include <cassert>

#include "ptr_container.h"

using namespace wxg;
using namespace std;

int main(){
	//case 1: access--> []&&at&&front&&back
	PtrContainer<int> ptrs{ new int(0), new int(2)};
	PtrContainer<int> ptrs_ = move(ptrs);
	assert(ptrs.empty());
	ptrs.push_back(new int(9));
	ptrs = move(ptrs_);
	assert(ptrs_.empty());
	cout << ptrs[0] << "\t" << ptrs[1] << '\n';
	cout << ptrs.at(0) << "\t" << ptrs.at(1) << '\n';
	cout << ptrs.front() << "\t" << ptrs.back() << '\n';

	//case 2: modification--> insert&&emplace_back&&clear 
	ptrs.clear();
	assert( ptrs.empty() );
	ptrs.emplace_back(3);
	ptrs.push_back(new int(4));
	// only movable ptr is supported in push_back
	auto tmp_ptr = new int(5);
	ptrs.push_back( move(tmp_ptr) );
	cout << ptrs.at(0) << "\t" << ptrs.at(1) << '\t' <<  ptrs[2] << '\n';

	//case 3: iterators--> begin&&end
	for( auto&& eptr : ptrs){
		cout << eptr << '\t';
	}
	cout << '\n';

	auto rit = ptrs.rbegin();
	while( rit!= ptrs.rend() ){
		cout << *rit << '\t';
		++rit;
	}
	cout << '\n';

	auto it = ptrs.begin();
	while( it!= ptrs.end() ){
		cout << *it << '\t';
		++it;
	}
	cout << '\n';
	auto const& cptrs = ptrs;
	auto crit = cptrs.crbegin();
	while( crit!= cptrs.crend() ){
		cout << *crit << '\t';
		++crit;
	}
	cout << '\n';
	auto cit = ptrs.cbegin();
	while( cit!= ptrs.cend() ){
		cout << *cit << '\t';
		++cit;
	}
	cout << '\n';

	//case4: compilation assurance
	enum class Type{ Classs, Enum};
	PtrContainer<Type>			enumCtn;
	union	Union { int c; double d;}; 
	PtrContainer<Union>			unionCtn;
	/*following type is not allowed
	PtrContainer<void>			voidCtn;
	PtrContainer<int*>			ptrCtn;
	PtrContainer<int(*)(int)>	pfCtn;
	PtrContainer<int(int)>		funcCtn;
	*/

	//case5: iterators
	class House{
		public:	
			House( std::string name) : m_name(name){}
			void Echo() {
				cout << "house's name is " << m_name << '\n';	
			}
			void Echo() const{
				cout << "house's const name is " << m_name << '\n';	
			}
		private:
			std::string	m_name;
	};
	PtrContainer<House> houseCtn;
	houseCtn.push_back(new House("zx"));
	houseCtn.push_back(new House("yy"));
	for( auto&& eptr : houseCtn){
		eptr.Echo();
	}
	auto hit = houseCtn.begin();
	auto h1 = hit;
	auto h2 = hit+1;
	assert(h1!=h2);
	assert( h1== hit++);
	assert( h2== hit);
	assert( h2== hit--);
	assert( h1== hit);
	assert( h2== ++hit);
	assert( h1== --hit);
	assert( h2== (hit+1) );
	assert( h1== hit );
	assert( h2== (hit+=1) );
	assert( h2== hit );
	assert( h1== (hit-1) );
	assert( h2== hit );
	assert( h1== (hit-=1) );
	assert( h1== hit );
	h1[0].Echo();
	h1[1].Echo();
	
	while( hit!= houseCtn.end() ){
		hit->Echo();
		++hit;
	}
	houseCtn.cbegin()->Echo();
	return 0;
}
