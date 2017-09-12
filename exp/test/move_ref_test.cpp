#include <iostream>

#include <functional>

class X 
{
	public:
		X(){}
		X( X&&)
		{
			std::cout << "move ctor invoked\n";	
		}
		X( X const&)
		{
			std::cout << "const ref ctor invoked\n";	
		}

		// non-const object can call const function 
		// but const function won't return non-const reference
		// if only a const version {Me} is provided
		// then non-const object will get const return value too
		// if only a non-const version {Me} is provided
		// then const object can't call 
		X const& Me() const
		{
			return *this;//m_val;	
		}

		void Set()
		{
			m_val = 3;	
			
		}

		int m_val = 1;
		
};

void call_move( X&& x)
{
	std::function<void(X&&)>	func = [](X&&){ std::cout << "func called\n";};	
	func( std::move(x)); // x is value type, not movable type
}

int main()
{
	// temperal -> &&
	call_move(X());	
	X x;
	// value -> &&
//	call_move(x); // nope	
	// value gets moved
	call_move(std::move(x));	

	X& ref_x = x;
//	call_move(ref_x); // nope	
	// ref gets moved
	call_move(std::move(ref_x));	

//	X const const_x = x;
//	call_move(const_x);	// nope
	// ref gets moved
//	call_move(std::move(const_x));	// nope

	X const& const_ref_x = x;
//	call_move(const_ref_x);	// nope
	//const ref gets moved
//	call_move(std::move(const_ref_x)); // nope	

/* *
 * conclusion:
 *		only temperal value can convert to && directly
 *		besides, value & ref can be moved by std::move
 *		at last, to pass &&t to a function which takes &&
 *			t should be moved by std::move 	
 *
 *		### const (ref) move converts to const value 
 *			by std::move rather than &&
 *
 *	template<class T>
 *	std::remove_reference_t<T>&& move( T&& t){
 *		return static_cast<std::remove_reference_t<decltype(t)>&&>(t);
 *	}
 *
 *	template<class T>
 *	T&& forword( remove_reference_<T>&/& t) noexcept{  // & for ref type, && for non-ref type
 *		return static_cast<decltype(t)&&>(t);
 *	}
 *
 *
 * */

	//test const T invoke {T& me() const}
	auto x2 = x.Me(); // const ref ctor is invoked
					// x is non-const, but it's return 
					// can be const
	x2.Set();
	auto & x3 = x.Me(); // x3 is const ref;
//	x3.Set(); // const object can't call non-const function
	

	/* *
	 * conclusion:
	 *		1> return auto can deduce constness and type, but not reference	
	 *		2> non-const object can call const member function and const function 
	 *			can only return const reference, so non-const object wants to 
	 *			return non-const reference, a non-const member function should be
	 *			defined;	const object can't call non-const member function
	 *			
	 * */

	/* * 
	 * decltype can deduce lvalue_ref or rvalue_ref but not constness
	 * */
	using XX = decltype(x3);	
	std::cout << std::boolalpha << std::is_lvalue_reference<XX>::value << '\n'; // true
	std::cout << std::boolalpha << std::is_const<XX>::value << '\n'; //false 
	std::cout << std::boolalpha << std::is_rvalue_reference<decltype(X())&&>::value << '\n'; // true


	return 0;
}
