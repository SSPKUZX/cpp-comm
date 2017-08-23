#pragma once

/*
 * simple version of retry
 *
 * */

#include <cstdint>
#include <type_traits>

namespace cmn
{
/*	template< class Predicate, class Callable, class... Args>
	auto retry( uint32_t const times, Predicate&& predicate, Callable&& caller, Args&&... args)
	-> typename std::result_of<Callable(Args...)>::type
	{
		using R = typename std::result_of<Callable(Args...)>::type;
		R ret;
		uint32_t time = 0;	
		do {
			ret = caller(std::forward<Args>(args)...);
		} while( ++time < times && !predicate(ret) );

		return ret;
	}
*/
	// better signature would be -> auto retry(invoker, predicate, times=DEFAULT_TIMES)
	//							OR  auto retry(invoker, times=DEFAULT_TIMES) when invoker returns bool
	// above parameter order is most natural : 1) retry what; 2) decide on success or failure 
	//										   3) retry times, if not set, use the default
	
	template<class Invoker, class Predicate, class RetType = typename std::result_of<Invoker()>::type>		
	auto retry( Invoker&& invoker, Predicate&& predicate, uint32_t times = 3)
	-> typename std::enable_if< !std::is_void<RetType>::value && !std::is_same<bool, RetType>::value, RetType>::type
	{
		RetType ret;		
		uint32_t current_time = 0;	
		do {
			ret = invoker();
		} while( ++current_time < times && !predicate(ret) );

		return ret;
	}

	template<class Invoker, class RetType = typename std::result_of<Invoker()>::type>		
	auto retry( Invoker&& invoker, uint32_t times = 3)
	-> typename std::enable_if< std::is_same<bool, RetType>::value, RetType>::type
	{
		RetType ret;		
		uint32_t current_time = 0;	
		do {
			ret = invoker();
		} while( ++current_time < times && !ret );

		return ret;
	}

	// assistant for retry(invoker, times) when '!invoker()' is needed   
	template<class Predicate>
	struct fnot_wrapper{
		fnot_wrapper( Predicate predicate) : m_pred(predicate){}		
		bool operator()(){ return !m_pred(); }

		typename std::decay<Predicate>::type m_pred;
	};

	template<class Predicate>
	inline fnot_wrapper<Predicate> fnot( Predicate&& pred){
		return fnot_wrapper<Predicate>(std::forward<Predicate>(pred) );			
	}

}
