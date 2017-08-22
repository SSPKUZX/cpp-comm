#pragma once

#include <functional> 
#include "traits_ext.h" 

namespace comm
{
	template< class Predicate >
	class times
	{
		public:
			times( uint32_t const max_times, Predicate const& pred); 
			template<class R>
			bool operator() ( R const& ret );

		private:
			Predicate	m_fPred;
			uint32_t	m_iMaxTimes;
			uint32_t	m_iTimes;
	};

	template<class Predicate>
	times<Predicate> make_times( uint32_t const max_times, Predicate&& pred );

	class retryer
	{
		public:
			template< class Predicate, class Callable, class... Args>
			auto operator()( Predicate&& predicate, Callable&& caller, Args&&... args)
				->typename std::enable_if< !std::is_same<void, typename std::result_of<Callable(Args...)>::type>::value 
				&& is_functional<Predicate, bool(typename std::result_of<Callable(Args...)>::type)>::value, 
				typename std::result_of<Callable(Args...)>::type>::type;
	};
	
	static retryer retry;

	// Predicate will take the R that the Callable returns as the only parameter
	// and return true if condition meets
/*	template< class Predicate, class Callable, class... Args>
	auto retry( Predicate&& predicate, Callable&& caller, Args&&... args)
		->typename std::enable_if< !std::is_same<void, typename std::result_of<Callable(Args...)>::type>::value 
		&& is_functional<Predicate, bool(typename std::result_of<Callable(Args...)>::type)>::value, 
		typename std::result_of<Callable(Args...)>::type>::type;
		*/

}// end of comm

#include "retry_inl.h"
