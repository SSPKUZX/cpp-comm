#pragma once

#include <algorithm>

namespace alg 
{
	namespace detail
	{
		template<class iterator>
		class findee;
	}
	// support for finding key for map/set 
	// and unique or specified element for sequential containers 
	template<class Container, class Equaler>
	auto find( Container const& container, Equaler const& equaler)
		->detail::findee< typename Container::const_iterator >;


	// transform SeqContainer<ST> to DestContainer<DT>
	template<template<class DT, class DAlloc = std::allocator<DT>> class DestContainer, 
			 template<class ST, class SAlloc = std::allocator<ST>> class SeqContainer, 
			 class ST, class F, class DT = typename std::result_of<F(ST&)>::type >
	inline DestContainer<DT> transform( SeqContainer<ST>& source, F&& transformer){
		DestContainer<DT> dest(source.size());
		std::transform(source.begin(), source.end(), dest.begin(), std::forward<F>(transformer));
		return std::move(dest);	
	}

	// transform SeqContainer<ST> to SeqContainer<DT>
	template<template<class T, class Alloc = std::allocator<T>> class SeqContainer, 
		class ST, class F, class DT = typename std::result_of<F(ST&)>::type >
	inline SeqContainer<DT> transform( SeqContainer<ST>& source, F&& transformer){
		return transform<SeqContainer,SeqContainer>(source, std::forward<F>(transformer));
	}

	// initializer_list, sequencial containers and sets, map excluded
	template<template<class T, class... Args> class ContainerType=std::initializer_list, class T>
	inline bool one_of( T const& val, ContainerType<T> const& container){
		return std::any_of(container.begin(), container.end(), std::bind1st(std::equal_to<T>(),val) );	
	}
	template<template<class T, class... Args> class ContainerType=std::initializer_list, class T>
	inline bool none_of( T const& val, ContainerType<T> const& container){
		return not one_of(val, container);
	}
}// end of alg 

#include "alg_inl.h"