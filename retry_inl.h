#pragma once

#include "call.h"

namespace comm
{
	template< class Predicate >
	times<Predicate>::times( uint32_t const max_times, Predicate const& pred) : m_iMaxTimes( max_times), m_fPred( pred ), m_iTimes(0) {};

	template<class Predicate>
	template<class R>
	bool times<Predicate>::operator() ( R const& ret )
	{
		return m_fPred( ret ) || ++m_iTimes >= m_iMaxTimes;	
	}

	template< class Predicate >
	times<Predicate> make_times( uint32_t const max_times, Predicate&& pred )
	{
		return times<Predicate>(max_times, std::forward<Predicate>(pred) );
	}

	template< class Predicate, class Callable, class... Args>
	auto retryer::operator()( Predicate&& predicate, Callable&& caller, Args&&... args)
		->typename std::enable_if< !std::is_same<void, typename std::result_of<Callable(Args...)>::type>::value 
		&& is_functional<Predicate, bool(typename std::result_of<Callable(Args...)>::type)>::value, 
		typename std::result_of<Callable(Args...)>::type>::type
	{
		return call_until( std::forward<Predicate>(predicate), std::forward<Callable>(caller), std::forward<Args>(args)... );
	}

	// Predicate will take the R that the Callable returns as the only parameter
	// and return true if condition meets
/*	template< class Predicate, class Callable, class... Args>
	auto retry( Predicate&& predicate, Callable&& caller, Args&&... args)
		->typename std::enable_if< !std::is_same<void, typename std::result_of<Callable(Args...)>::type>::value 
		&& is_functional<Predicate, bool(typename std::result_of<Callable(Args...)>::type)>::value, 
		typename std::result_of<Callable(Args...)>::type>::type
	{
		return call_until( std::forward<Predicate>(predicate), std::forward<Callable>(caller), std::forward<Args>(args)... );
	}*/

} // end of comm
	
