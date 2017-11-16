#pragma once

#include <string>
#include <sstream>

namespace utl{
	template<class T>
	std::string lexical_cast(T const& t){
		static thread_local std::ostringstream oss;	
		oss.str("");
		oss << t;
		return oss.str();
	}

	/*
	 * T&			-> T&
	 * const T&		-> T&
	 */
	template<class T>
	inline auto unconst(T&& val)
		-> typename std::enable_if<std::is_lvalue_reference<T>::value,
		typename std::add_lvalue_reference<typename std::decay<T>::type>::type>::type
	{
		return const_cast<typename std::add_lvalue_reference<typename std::decay<T>::type>::type>(std::forward<T>(val));	
	}
	
	/*
	 * T&&			-> T&&
	 * const T&&	-> T&&
	 */
	template<class T>
	inline auto unconst(T&& val)
		-> typename std::enable_if<std::is_rvalue_reference<T>::value,
		typename std::add_rvalue_reference<typename std::decay<T>::type>::type>::type
	{
		return const_cast<typename std::add_rvalue_reference<typename std::decay<T>::type>::type>(std::forward<T>(val));	
	}
	
	/* 
	 * T			-> T
	 * const T		-> T
	 */
	template<class T>
	inline auto unconst(T&& val)
		-> typename std::enable_if<not std::is_reference<T>::value,
		typename std::remove_const<T>::type>::type
	{
		return const_cast<typename std::remove_const<T>::type>(std::forward<T>(val));	
	}


}
