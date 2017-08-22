#pragma once

#include "traits_ext.h" 
#include <stdint.h>
#include <memory>

namespace comm
{
	// for each key, the callable will be called only once even invoked multiple times
	template< class KeyType, class Callable, class... Args>
	void call_once_per_key( KeyType _key, Callable&& caller, Args&&... args);

	// the callable will be called only once on no condition
	template< class Callable, class... Args>
	void call_once_only( Callable&& caller, Args&&... args);

	// called when true and return void
	template< class Callable, class... Args>
	auto call_if( bool cond, Callable&& caller, Args&&... args)
		->typename std::enable_if< std::is_same<void, typename std::result_of<Callable(Args...)>::type >::value, void>::type;

	// called when true and return shared_ptr<R> 
	template< class Callable, class... Args>
	auto call_if( bool cond, Callable&& caller, Args&&... args)
		->typename std::enable_if< !std::is_same<void, typename std::result_of<Callable(Args...)>::type >::value,
		  std::shared_ptr<typename std::result_of<Callable(Args...)>::type> >::type;

	// when predicate returns true, it will be called
	template< class Predicate, class Callable, class... Args>
	auto call_if( Predicate&& predicate, Callable&& caller, Args&&... args)
		->decltype( call_if( predicate(), std::forward<Callable>(caller), std::forward<Args>(args)... ) );

	// return void, until predicate() evaluates to true 
	template< class Predicate, class Callable, class... Args>
	auto call_until( Predicate&& predicate, Callable&& caller, Args&&... args)
		->typename std::enable_if< is_functional<Predicate,bool(void)>::value, void>::type;

	// return void, until predicate(R) evaluates to true when callable returns R
	template< class Predicate, class Callable, class... Args>
	auto call_until( Predicate&& predicate, Callable&& caller, Args&&... args)
		->typename std::enable_if< !std::is_same<void, typename std::result_of<Callable(Args...)>::type>::value 
		&& is_functional<Predicate, bool(typename std::result_of<Callable(Args...)>::type)>::value, 
		typename std::result_of<Callable(Args...)>::type>::type;

	// call multiple times
	template< class Callable, class... Args>
	void call_multiple( uint32_t const times, Callable&& caller, Args&&... args);

	// for each argument in variadic parameter 'args', it will be called by callable
	// when callable returns bool, call_each returns bool type and returns true only when 
	// all invocations return true
	// otherwise, call_each returns void type
	template< class _Callable, class... _Args>
	auto call_each( _Callable&& callable, _Args&&... args)
		->typename std::conditional< std::is_same<bool,typename std::result_of< _Callable(typename get_type<0, _Args...>::type )>::type>::value, bool, void>::type;

}

#include "call_inl.h"
