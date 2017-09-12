/*
 * 1) auto&& and decltype(u) could deduce constness and reference, 
 *    auto only deduce type T but not constness and reference
 * 2) void func(T&&):  T is forwarded as T, or T& or T const&, 
 *    but never const T or T&&( equal to T)
 * 3) T const& is reference to const T, so remove_const<T const&>
 *	  results in T const& other than T&
 * */

#include <iostream>
#include <cassert>

class Tab
{
	public:
			Tab& Self() {
				return *this;	
			}

			Tab const& Self() const{
				return *this;	
			}

			void Say(){
					std::cout << "non-const say\n";	
			}

			void Say() const{
					std::cout << "const say\n";	
			}

			/* ambiguous to Echo(int) 
			void Echo( int&& k){
				std::cout << "rvalue Echo\n";	
			}
			*/
			void Echo( int k){
				std::cout << "value Echo\n";	
			}

			template<class U>
			void Deduce(U&& u){
				std::cout << "is_lvalue_reference:" <<  std::is_lvalue_reference<U>::value << '\n'	\
						  << "is_rvalue_reference:" <<  std::is_rvalue_reference<U>::value << '\n'	\
						  // always false there no matter U is T, T&, T const&, const T will never be here
						  << "is_const:" <<  std::is_const<U>::value << '\n';  	
				u.Say();
			}
};

int main(){
		Tab t;
		Tab const& c_t =t;
		// auto&& can deduce type, reference and constness
		auto&& _t = t.Self();
		assert( &_t == &t);
		auto&& _c_t = c_t.Self();
		assert( &_c_t == &c_t);
		_t.Say();
		_c_t.Say();

		// auto only deduce type, but not reference
		auto __t = _t;
		__t.Say();
		assert( &__t != &_t );

		// auto only deduce type, but not reference and constness
		auto __c_t = _c_t;
		__c_t.Say();
		assert( &__c_t != &_c_t );

		/* T and T&& could result in ambiguous invocation
		t.Echo(3);
		int k=9;
		t.Echo(std::move(k));
		*/

		t.Deduce(t); // T&& can deduce lvalue_reference
		t.Deduce(c_t); // T const& is not const by is_const.........
		std::cout << std::is_const<const Tab>::value << '\t' \
				  << std::is_const<Tab const&>::value << '\t'
				  << std::is_const<const Tab &>::value << '\n';
		const Tab c_t_;
		t.Deduce(c_t_); // T&& can deduce constness
		t.Deduce(std::move(t)); //T&& take rvalue as value
		t.Deduce(std::move(c_t_)); //T&& take rvalue as value and deduce constness as well

		// decltype could deduce constness and reference
		std::cout << std::boolalpha << "decltype's type:" << std::is_same<Tab, decltype(t)>::value << '\t'	
				  << std::is_same<Tab&, decltype(_t)>::value << '\t'
				  << std::is_same<Tab, decltype(Tab())>::value << '\t'
				  << std::is_same<Tab const&, decltype(c_t)>::value << '\n';


		return 0;
}

