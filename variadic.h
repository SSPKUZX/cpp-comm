#pragma once

#include <utility>
#include <cstdint>

namespace comm
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

	/* *
	 * *****index_of*******
	 * */

	template<class T,class... Args>
	struct index_of{};

	template<class T>
	struct index_of<T>
	{
		static constexpr int8_t	value = -1; 	
	};

	template<class T, class U, class... Args>
	struct index_of<T, U, Args...>
	{
		static constexpr int8_t value = std::is_same<T,U>::value ?
			0 : (-1 != index_of<T,Args...>::value) ? 
			(1 + index_of<T,Args...>::value) : -1;
	};

	template<class T, class... Args>
	struct one_of
	{
		static constexpr bool value = -1!=index_of<T,Args...>::value;
	};

	template<class... Args>
	struct has_duplicate{};

	template<>
	struct has_duplicate<>
	{
		static constexpr bool value = false; 
	};

	template<class T, class... Args>
	struct has_duplicate<T, Args...> 
	{
		static constexpr bool value = one_of<T,Args...>::value || has_duplicate<Args...>::value;
	};


	/* *
	 * *****nth_type*******
	 * */

	template<int32_t N, class Tp, class... Types>
	struct nth_type
	{
		static_assert( N>0, "N must be positive in nth_type");
		using type = typename nth_type<N-1, Types...>::type;		
	};

	template<class Tp, class... Types>
	struct nth_type<1, Tp, Types...>
	{
		using type = Tp;		
	};

} // end of comm
