#pragma once

#include <algorithm>

namespace comm
{
	namespace detail
	{
		template<class iterator>
		class find_result;
	}
	template<class _Container, class _Equaler>
	auto find( _Container const& container, _Equaler const& equaler)
		->detail::find_result< typename _Container::const_iterator >;


	template<template<class DT, class DAlloc = std::allocator<DT>> class DestContainer, 
			 template<class ST, class SAlloc = std::allocator<ST>> class SeqContainer, 
			 class ST, class F, class DT = typename std::result_of<F(ST&)>::type >
	inline DestContainer<DT> transform( SeqContainer<ST>& source, F&& transformer){
		DestContainer<DT> dest(source.size());
		std::transform(source.begin(), source.end(), dest.begin(), std::forward<F>(transformer));
		return std::move(dest);	
	}

	template<template<class T, class Alloc = std::allocator<T>> class SeqContainer, 
		class ST, class F, class DT = typename std::result_of<F(ST&)>::type >
	inline SeqContainer<DT> transform( SeqContainer<ST>& source, F&& transformer){
		return transform<SeqContainer,SeqContainer>(source, std::forward<F>(transformer));
	}

}// end of comm

#include "stl_inl.h"
