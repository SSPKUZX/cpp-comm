#include <cassert>

/*
 * T			-> T
 * T&&			-> T&&
 * const T		-> T
 * const T&&	-> T&&
 * T&			-> T&
 * const T&		-> T&
 *
 * */
template<class T>
inline auto drop_const(T&& val)
	-> typename std::enble_if<std::is_lvalue_reference<T>::value,
	typename std::add_lvalue_reference<typename std::decay<T>::type>::type>::type
{
	return const_cast<typename std::add_lvalue_reference<typename std::decay<T>::type>::type>(std::forward<T>(val));	
}

template<class T>
inline auto drop_const(T&& val)
	-> typename std::enble_if<std::is_rvalue_reference<T>::value,
	typename std::add_rvalue_reference<typename std::decay<T>::type>::type>::type
{
	return const_cast<typename std::add_rvalue_reference<typename std::decay<T>::type>::type>(std::forward<T>(val));	
}

template<class T>
inline auto drop_const(T&& val)
	-> typename std::enble_if<not std::is_reference<T>::value,
	typename std::remove_const<T>::type>::type
{
	return const_cast<typename std::remove_const<T>::type>>(std::forward<T>(val));	
}
