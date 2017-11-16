#pragma once

#include <algorithm>

namespace alg 
{
	namespace detail
	{
		template<class InputIterator, class T=typename std::iterator_traits<InputIterator>::value_type>
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
	template<template<class, class...> class ContainerType=std::initializer_list, class T>
	inline bool is_any_of( T const& val, ContainerType<T> const& container){
		return std::any_of(container.begin(), container.end(), std::bind1st(std::equal_to<T>(),val) );	
	}
	template<template<class, class...> class ContainerType=std::initializer_list, class T>
	inline bool is_none_of( T const& val, ContainerType<T> const& container){
		return not is_any_of(val, container);
	}

	template<template<class...> class MapType, class KeyType, class ValueType, class... Args>	
	std::vector<KeyType> KeysOf(MapType<KeyType,ValueType,Args...> const& mapValue){
		std::vector<KeyType> keys;	
		keys.reserve(mapValue.size());
		std::transform(mapValue.begin(), mapValue.end(), std::back_inserter(keys),
				[](std::pair<KeyType const, ValueType> const& p){ return p.first;});
		return std::move(keys);
	}	

	template<template<class...> class MapType, class KeyType, class ValueType, class... Args>	
	std::vector<ValueType> ValuesOf(MapType<KeyType,ValueType,Args...> const& mapValue){
		std::vector<ValueType> values;	
		values.reserve(mapValue.size());
		std::transform(mapValue.begin(), mapValue.end(), std::back_inserter(values),
				[](std::pair<KeyType const, ValueType> const& p){ return p.second;});
		return std::move(values);
	}	
}// end of alg 

#include "alg_inl.h"
