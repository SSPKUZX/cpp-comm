#pragma once

#include <future> // future,this_thread
#include <vector>
#include <algorithm> // transform,back_inserter,ostream_iterator

#include "traits.h"

namespace utl{

	template<class T>
	std::vector<T> WhenAll(std::vector<std::future<T>> futures ){
		std::vector<T>	results;
		std::transform(futures.begin(), futures.end(), std::back_inserter(results), 
					[](std::future<T>& fut){ return fut.get(); }
				);
		return std::move(results);
	} 
	
	/* begin/end of initializer_list is const, so {fut.get()} can't invoke
	template<class T>
	std::vector<T> WhenAll(std::initializer_list<std::future<T>> futures ){
		std::vector<T>	results;
		std::transform(futures.begin(), futures.end(), std::back_inserter(results), 
					[](std::future<T> const& fut) { return fut.get(); }
				);
		return std::move(results);
	} 
	*/
	
	
	template<class... Args>
	auto WhenAll( std::future<Args>&... futs )
		-> typename std::enable_if< not is_same_types<Args...>::value, 
									std::tuple<Args...> >::type{
		return std::make_tuple( futs.get()... );
	}
	
	template<class... Ts>
	auto WhenAll( std::future<Ts>&... futs)
		-> typename std::enable_if<is_same_types<Ts...>::value, 
		std::vector<typename std::common_type<Ts...>::type> >::type{
		return {futs.get()...};
	}

}

