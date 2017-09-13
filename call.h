#pragma once

#include <mutex>
#include <unordered_map>
#include <functional>
#include <stdint.h>
#include <memory>

#include "variadic.h"
#include "traits.h" 

namespace utl 
{
	// for each key, the callable will be called only once even invoked multiple times
	template< class KeyType, class Callable, class... Args>
	void call_once_per_key( KeyType _key, Callable&& caller, Args&&... args)
	{
		static std::unordered_map< KeyType, std::once_flag> s_key2flag;
		return std::call_once( s_key2flag[_key], std::forward<Callable>(caller), std::forward<Args>(args)... );
	}

	// the callable will be called only once on no condition
	template< class Callable, class... Args>
	inline void call_once_only( Callable&& caller, Args&&... args)
	{
		static std::once_flag s_once_flag;
		return std::call_once( s_once_flag, std::forward<Callable>(caller), std::forward<Args>(args)... );
	}

	// called when true and return void
	template< class Callable, class... Args>
	inline auto call_if( bool cond, Callable&& caller, Args&&... args)
		->enable_if_t< std::is_same<void, result_of_t<Callable(Args...)> >::value, void>
    {	
		if( cond ) 
			return caller( std::forward<Args>(args)... );
	}

	// called when true and return shared_ptr<R> 
	template< class Callable, class... Args>
	inline auto call_if( bool cond, Callable&& caller, Args&&... args)
		->enable_if_t< !std::is_same<void, result_of_t<Callable(Args...)> >::value,
		  std::shared_ptr<result_of_t<Callable(Args...)>> >
    {	
		using R = result_of_t<Callable(Args...)>;
		if( cond ) 
			return std::make_shared<R>( caller(std::forward<Args>(args)...) );
		else return std::shared_ptr<R>();
	}

	// when predicate returns true, it will be called
	template< class Predicate, class Callable, class... Args>
	inline auto call_if( Predicate&& predicate, Callable&& caller, Args&&... args)
		->decltype( call_if( predicate(), std::forward<Callable>(caller), std::forward<Args>(args)... ) )
    {	
		return call_if( predicate(), std::forward<Callable>(caller), std::forward<Args>(args)... );
	}

	// return void, until predicate() evaluates to true 
	template< class Predicate, class Callable, class... Args>
	inline auto call_until( Predicate&& predicate, Callable&& caller, Args&&... args)
		->enable_if_t< is_signature<Predicate, bool()>::value, void>
	{
		do {
			caller(std::forward<Args>(args)... );
		} while( !predicate() );
	}

	// return void, until predicate(R) evaluates to true when callable returns R
	template< class Predicate, class Callable, class... Args>
	inline auto call_until( Predicate&& predicate, Callable&& caller, Args&&... args)
		->enable_if_t< !std::is_same<void, result_of_t<Callable(Args...)>>::value 
		  && is_signature<Predicate, bool(result_of_t<Callable(Args...)>)>::value, 
		  result_of_t<Callable(Args...)>>
	{
		using R = result_of_t<Callable(Args...)>;
		R ret;
		do {
			ret = caller(std::forward<Args>(args)... );
		} while( !predicate(ret) );

		return ret;
	}

	// call multiple times
	template< class Callable, class... Args>
	inline void call_multiple( uint32_t const times, Callable&& caller, Args&&... args)
	{
		uint32_t _th = 0; 
		call_until( [&_th,&times]{ return ++_th>=times;}, std::forward<Callable>(caller), std::forward<Args>(args)... );
	}

	// for each argument in variadic parameter 'args', it will be called by callable
	// when callable returns bool, call_each returns bool type and returns true only when 
	// all invocations return true
	// otherwise, call_each returns void type
	template< class Callable, class... Args>
	inline auto call_each( Callable&& callable, Args&&... args)
		->conditional_t< std::is_same<bool,result_of_t< Callable(nth_type_t<0,Args...> )>>::value, bool, void>
	{
		return vparams(std::forward<Callable>(callable), std::forward<Args>(args)...);
	}

} // end of utl 

