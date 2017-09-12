#pragma once

#include <utility>
#include <cstdint>

namespace utl 
{
	namespace detail
	{
		// variadic_np --> [n]ot as function [p]amameter but pure template parameter
		// example usage, please refer to arch/csp/type_indexes_of && instances_of
		template<template<class> class Hook, class... Types>
		class variadic_np{};

		template<template<class >class Hook, class T, class... Types>
		class variadic_np<Hook, T, Types...> // : public variadic_np<Hook, Types...>
		{
			public:
				template<class... Args>
				void operator()( Args&&... args)
				{
					Hook<T>()( std::forward<Args>(args)... );
					variadic_np<Hook, Types...>()(std::forward<Args>(args)... );	
				}
		};
		
		template<template<class >class Hook>
		class variadic_np<Hook>
		{
			public:
				template<class... Args>
				void operator()( Args&&... args){}
		};


		// variadic_ap --> variadic template parameter [a]s function [p]arameter
		// used by call_each in call_inl.h
		
		// specialize for single parameter
		template< class Callable, class Tp, class... Args >
		auto variadic_ap( Callable&& callable, Tp&& tp, Args&&... args )
			->typename std::enable_if< sizeof...(Args) == 0, typename std::result_of< Callable(Tp) >::type >::type
		{
			return callable( std::forward<Tp>(tp) );
		}

		// specialize for bool return type
		template< class Callable, class Tp, class... Args>
		auto variadic_ap( Callable&& callable, Tp&& tp, Args&&... args)
			->typename std::enable_if< (sizeof...(Args) > 0) && std::is_same<bool,typename std::result_of< Callable(Tp) >::type >::value, bool>::type
		{
			return callable( std::forward<Tp>(tp) ) && variadic_ap( std::forward<Callable>(callable), std::forward<Args>(args)...);
		}

		// if return type is not bool, then return void
		template< class Callable, class Tp, class... Args>
		auto variadic_ap( Callable&& callable, Tp&& tp, Args&&... args)
			->typename std::enable_if< (sizeof...(Args) > 0) && !std::is_same<bool,typename std::result_of< Callable(Tp) >::type >::value, void>::type
		{
			callable( std::forward<Tp>(tp) );
			variadic_ap( std::forward<Callable>(callable), std::forward<Args>(args)... );
		}

	} // end of detail


} // end of utl 
