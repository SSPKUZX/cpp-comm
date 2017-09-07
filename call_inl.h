#pragma once

#include <mutex>
#include <unordered_map>
#include <functional>
#include "variadic.h"

namespace utl 
{
	template< class KeyType, class Callable, class... Args>
	void call_once_per_key( KeyType _key, Callable&& caller, Args&&... args)
	{
		static std::unordered_map< KeyType, std::once_flag> s_key2flag;
		return std::call_once( s_key2flag[_key], std::forward<Callable>(caller), std::forward<Args>(args)... );
	}

	template< class Callable, class... Args>
	void call_once_only( Callable&& caller, Args&&... args)
	{
		static std::once_flag s_once_flag;
		return std::call_once( s_once_flag, std::forward<Callable>(caller), std::forward<Args>(args)... );
	}

	template< class Callable, class... Args>
	auto call_if( bool cond, Callable&& caller, Args&&... args)
		->typename std::enable_if< std::is_same<void, typename std::result_of<Callable(Args...)>::type >::value, void>::type
    {	
		if( cond ) 
			return caller( std::forward<Args>(args)... );
	}

	template< class Callable, class... Args>
	auto call_if( bool cond, Callable&& caller, Args&&... args)
		->typename std::enable_if< !std::is_same<void, typename std::result_of<Callable(Args...)>::type >::value,
		  std::shared_ptr<typename std::result_of<Callable(Args...)>::type> >::type
    {	
		using R = typename std::result_of<Callable(Args...)>::type;
		if( cond ) 
			return std::make_shared<R>( caller(std::forward<Args>(args)...) );
		else return std::shared_ptr<R>();
	}

	template< class Predicate, class Callable, class... Args>
	auto call_if( Predicate&& predicate, Callable&& caller, Args&&... args)
		->decltype( call_if( predicate(), std::forward<Callable>(caller), std::forward<Args>(args)... ) )
    {	
		return call_if( predicate(), std::forward<Callable>(caller), std::forward<Args>(args)... );
	}

	template< class Predicate, class Callable, class... Args>
	auto call_until( Predicate&& predicate, Callable&& caller, Args&&... args)
		->typename std::enable_if< is_signature<Predicate, bool()>::value, void>::type
	{
		do {
			caller(std::forward<Args>(args)... );
		} while( !predicate() );
	}

	template< class Predicate, class Callable, class... Args>
	auto call_until( Predicate&& predicate, Callable&& caller, Args&&... args)
		->typename std::enable_if< !std::is_same<void, typename std::result_of<Callable(Args...)>::type>::value 
		  && is_signature<Predicate, bool(typename std::result_of<Callable(Args...)>::type)>::value, 
		  typename std::result_of<Callable(Args...)>::type>::type
	{
		using R = typename std::result_of<Callable(Args...)>::type;
		R ret;
		do {
			ret = caller(std::forward<Args>(args)... );
		} while( !predicate(ret) );

		return ret;
	}

	template< class Callable, class... Args>
	void call_multiple( uint32_t const times, Callable&& caller, Args&&... args)
	{
		uint32_t _th = 0; 
		call_until( [&_th,&times]{ return ++_th>=times;}, std::forward<Callable>(caller), std::forward<Args>(args)... );
	}

	template< class _Callable, class... _Args>
	auto call_each( _Callable&& callable, _Args&&... args)
		->typename std::conditional< std::is_same<bool,typename std::result_of< _Callable(typename nth_type<0,_Args...>::type )>::type>::value, bool, void>::type
	{
		return detail::variadic_ap(std::forward<_Callable>(callable), std::forward<_Args>(args)...);
	}

} // end of comm

