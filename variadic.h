#pragma once

#include "traits.h"

namespace utl 
{
	// vtypes_helper --> [n]ot as function [p]amameter but pure template parameter
	// example usage, please refer to cmf/comm/type_indexes_of && instances_of
	template<template<class> class Hook, class... Types>
	class vtypes{};

	template<template<class >class Hook, class T, class... Types>
	class vtypes<Hook, T, Types...> 
	{
		public:
			template<class... Args>
			inline void operator()( Args&&... args)
			{
				Hook<T>()( std::forward<Args>(args)... );
				vtypes<Hook, Types...>()(std::forward<Args>(args)... );	
			}
	};
	
	template<template<class >class Hook>
	class vtypes<Hook>
	{
		public:
			template<class... Args>
			inline void operator()( Args&&... args){}
	};

	template<class T>
	struct TypeInfoHook{
		inline void operator()(std::function<void(std::type_info const& tinfo)> hook){
			hook(typeid(T));	
		}			
	};

	template<class... Types>
	inline void vtypeinfos( std::function<void(std::type_info const& tinfo)> const& hook) {
		vtypes<TypeInfoHook, Types...>()(hook);
	}

	// variadic_ap --> variadic template parameter [a]s function [p]arameter
	// used by call_each in call.h
	
	// specialize for single parameter
	template< class Callable, class Tp, class... Args >
	inline auto vparams(Callable&& callable, Tp&& tp, Args&&... args )
		->enable_if_t< sizeof...(Args) == 0, result_of_t< Callable(Tp) > >
	{
		return callable( std::forward<Tp>(tp) );
	}

	// specialize for bool return type
	template< class Callable, class Tp, class... Args>
	inline auto vparams( Callable&& callable, Tp&& tp, Args&&... args)
		->enable_if_t< (sizeof...(Args) > 0) && std::is_same<bool,result_of_t< Callable(Tp) > >::value, bool>
	{
		return callable( std::forward<Tp>(tp) ) && vparams( std::forward<Callable>(callable), std::forward<Args>(args)...);
	}

	// if return type is not bool, then return void
	template< class Callable, class Tp, class... Args>
	inline auto vparams( Callable&& callable, Tp&& tp, Args&&... args)
		->enable_if_t< (sizeof...(Args) > 0) && !std::is_same<bool,result_of_t< Callable(Tp) > >::value, void>
	{
		callable( std::forward<Tp>(tp) );
		vparams( std::forward<Callable>(callable), std::forward<Args>(args)... );
	}

} // end of utl 
