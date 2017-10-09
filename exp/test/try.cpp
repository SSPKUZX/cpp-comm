/**
 * ananas/future/future/promise.setValue 
 * 7 setValues there could summerize as one
 * through following tricks
 *		1> class partial specialization for different type signatures
		2> is_same<const int&, typename std::remove_const<const int&>::type>::value == true 
		3> as function parameter, T is used when T&& specialization exists too
 */

#include <iostream>
#include <cassert>

using namespace std;


#define REGISTER_MSG_CB(msg_type, received_cb)	\
	void registrar_##__COUNTER__(int k){        \
		received_cb(k);							\
	}
	
template<class T>
class Try{
	static_assert( !std::is_reference<T>::value&&!std::is_const<T>::value, "T can't be reference or const");

	public:
		//support implicit conversion
		Try( T&& t) : m_val(std::move(t)){}
		Try( T const& t) : m_val(t){}

		void operator() () const{
			cout << "inner value is:" << m_val << '\n';	
		}

	private:
		T	m_val;
};

template<>
class Try<void>{
	public:
		void operator() () const{
			cout << "try<void> operator()\n";	
		}
};

/* ananas/future/promise.setValue could use this to simplify and unify the code
template<class... Args>
void wrap_try(Args&&... args){}
*/

template<class... Args>
struct valid_try_param{
	static constexpr const char* descriptor = "default-annotation";
};

template<>
struct valid_try_param<>{
	using try_type = Try<void>;
	static Try<void> wrap_try(){ return Try<void>{}; }
	static constexpr const char* descriptor = "empty void type";
};

template<class T> 
struct valid_try_param<T>{
	using try_type = Try<T>;
	static Try<T> wrap_try(T t){ return Try<T>(move(t)); }
	static constexpr const char* descriptor = "value type";
};

/* equals to value type
template<class T> //, class X=typename std::enable_if<std::is_rvalue_reference<U>::value>::type >
struct valid_try_param<T&&>{
	enum{ value=true };
	using type=T;
	static constexpr const char* descriptor = "rvalue_reference";
};
*/
template<class T>
struct valid_try_param<T&>{ 
	using try_type = Try<T>;
	static Try<T> wrap_try(T& t){ return Try<T>(t); }
	static constexpr const char* descriptor = "lvalue_reference";
};

template<class T>
struct valid_try_param<T const&>{ 
	using try_type = Try<T>;
	static Try<T> wrap_try(T const& t){ return Try<T>(t); }
	static constexpr const char* descriptor = "const_lvalue_reference";
};

template<class T>
struct valid_try_param<Try<T>>{
	using try_type = Try<T>;
	static Try<T> wrap_try(Try<T> t){ return move(t); }
	static constexpr const char* descriptor = "value_type try<T>";
};
/*
template<class T>//, typename std::enable_if<std::is_rvalue_reference<U>::value&&is_try<U>::value>::type>
struct valid_try_param<Try<T>&&>{
	enum{ value=true };
	using type=T;
	static constexpr const char* descriptor = "rvalue_reference try<T>";
};
*/
template<class T>
struct valid_try_param<Try<T>&>{
	using try_type = Try<T>;
	static Try<T> wrap_try(Try<T>& t){ return t; }
	static constexpr const char* descriptor = "lvalue_reference try<T>";
};

template<class T>
struct valid_try_param<Try<T> const&>{
	using try_type = Try<T>;
	static Try<T> wrap_try(Try<T> const& t){ return t; }
	static constexpr const char* descriptor = "const_lvalue_reference try<T>";
};

template<>
struct valid_try_param<Try<void>>{
	using try_type = Try<void>;
	static Try<void> wrap_try(Try<void> t){ return move(t); }
	static constexpr const char* descriptor = "value_type try<void>";
};

template<>
struct valid_try_param<Try<void> const&>{
	using try_type = Try<void>;
	static Try<void> wrap_try(Try<void> const& t){ return t; }
	static constexpr const char* descriptor = "const_lvalue_reference try<void>";
};

template<>
struct valid_try_param<Try<void>&>{
	using try_type = Try<void>;
	static Try<void> wrap_try(Try<void>& t){ return t; }
	static constexpr const char* descriptor = "lvalue_reference try<void>";
};


template<class... U>
typename valid_try_param<U...>::try_type Valid(U&&... u){
	static_assert( (sizeof...(U) <= 1), "length of variadic U... must be only one or zero");
	cout << valid_try_param<U...>::descriptor << '\n';	
	return valid_try_param<U...>::wrap_try(std::forward<U>(u)...);
}
//is_same<const int&, typename std::remove_const<const int&>::type>::value == true 
template<class T>
using raw_type_t = typename std::remove_const<typename std::remove_reference<T>::type>::type;

template<class T, class X = raw_type_t<T>>
auto Echo(T&& t) ->typename std::enable_if<std::is_same<X, int>::value>::type
{
	cout << "raw_type X enabled Echo:" << t << '\n';
}

template<class T>
struct is_try : public std::integral_constant<bool, false>{};

template<class T>
struct is_try<Try<T>> : public std::integral_constant<bool, true>{
	using value_type = T;
};

template<class T>
struct is_try<Try<T>&> : public std::integral_constant<bool, true>{
	using value_type = T;
};

template<class T>
struct is_try<Try<T> const&> : public std::integral_constant<bool, true>{
	using value_type = T;
};

// T, Try<T>,Try<void>  --> move
template<class T>
inline auto setValue(T&& t) ->typename std::enable_if< !std::is_lvalue_reference<T>::value, T&& >::type {      
	return std::move(t); 	
}
// T&, T const&,  Try<T>&, Try<T> const&, Try<void>&, Try<void> const&---> as exact as it is 
template<class T>
inline auto setValue(T&& t) ->typename std::enable_if< std::is_lvalue_reference<T>::value, T >::type {      
	return t; 
}
// empty  ---> Try<void> 
inline Try<void> setValue(){ return Try<void>(); }

int main(){
	// T, Try<T>,Try<void>  --> move
	cout << boolalpha << std::is_same<int&&, decltype(setValue(3)) >::value << '\n';	
	cout << boolalpha << std::is_same<Try<int>&&, decltype(setValue(Try<int>(3))) >::value << '\n';	
	cout << boolalpha << std::is_same<Try<void>&&, decltype(setValue(Try<void>())) >::value << '\n';	

	// T&, T const&,  Try<T>&, Try<T> const&, Try<void>&, Try<void> const&---> as exact as it is 
	int x=9;
	cout << boolalpha << std::is_same<int&, decltype(setValue(x)) >::value << '\n';	
	const int y=9;
	cout << boolalpha << std::is_same<int const&, decltype(setValue(y)) >::value << '\n';	
	Try<int> a(3);
	cout << boolalpha << std::is_same<Try<int>&, decltype(setValue(a)) >::value << '\n';	
	const Try<int> b(3);
	cout << boolalpha << std::is_same<Try<int> const&, decltype(setValue(b)) >::value << '\n';	
	Try<void> c;
	cout << boolalpha << std::is_same<Try<void>&, decltype(setValue(c)) >::value << '\n';	
	const Try<void> d;
	cout << boolalpha << std::is_same<Try<void> const&, decltype(setValue(d)) >::value << '\n';	

	// empty  ---> Try<void> 
	cout << boolalpha << std::is_same<Try<void>, decltype(setValue()) >::value << '\n';	

	cout << is_same<int, raw_type_t<int const&>>::value << '\n';
	cout << is_same<int, raw_type_t<const int&>>::value << '\n';
	cout << is_same<int const&, typename std::remove_const<int const&>::type>::value << '\n';
	cout << is_same<const int&, typename std::remove_const<const int&>::type>::value << '\n';


	Valid(3)(); //value typed
	int k=9;
	Valid(move(k))(); // value typed
	const int& m = k;
	Valid(m)(); // reference typed
	int& n = k;
	Valid(n)(); // reference typed

	Echo(n);

	Valid(Try<int>(5))(); // value type
	Try<int> ti(6);
	Valid(ti)(); // lvalue_reference
	auto const& cti = ti;
	Valid(cti)(); // const_lvalue_reference
	auto& rti = ti;
	Valid(rti)(); // lvalue_reference

	Valid(Try<void>())(); // value type
	Try<void> tv;
	Valid(tv)(); // lvalue_reference
	auto const& ctv = tv;
	Valid(ctv)(); // const_lvalue_reference
	auto& rtv = tv;
	Valid(rtv)(); // lvalue_reference

	Valid()();

	return 0;
}

