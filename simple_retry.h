#pragma once

/*
 * 
 *
 * */

#include <cstdint>
#include <type_traits>

namespace cmn
{
	template< class Predicate, class Callable, class... Args>
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
}
