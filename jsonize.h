#pragma once

#include <vector>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <initializer_list>

namespace json
{
	using json_type = std::string;

	static std::unordered_map< std::type_index, std::vector<std::string> > g_type2info;

	// register type's property name list to use in serialization and deserialization
	inline void reg_type( std::type_index const& type, std::vector<std::string> const& meta_info )
	{ g_type2info.emplace( type, meta_info); }
	inline void reg_type( std::type_index const& type, std::initializer_list<std::string> const& meta_info )
	{ g_type2info.emplace( type, std::vector<std::string>(meta_info) ); }

	// uniform serialization interface
	template<class _Tp>
	bool obj2json( _Tp const& obj, json_type& json){ return false; };

	// help function for obj2json
	template<class... _Args>
	bool ser( json_type& json, std::type_index const& type, _Args const&... args );


	// uniform deserialization interface
	template<class _Tp>
	bool json2obj( json_type const& json, _Tp& obj){ return false; };
	
	// help function for json2obj
	template<class... _Args>
	bool des( json_type const& json, std::type_index const& type, _Args&... args);

}; // end of json

#include "jsonize_basic.h"
#include "jsonize_inl.h"
