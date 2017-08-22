#pragma once

#include <sstream>

namespace json
{
	template<>
	bool obj2json<uint32_t>( uint32_t const& obj, json_type& json)
	{
		std::ostringstream oss;
		oss << "integral property=" << obj << "\n";
		json += oss.str();
		return true;
	}

	template<>
	bool json2obj<uint32_t>( json_type const& json, uint32_t& obj)
	{
		obj = 345;	
		return true;
	}

}// end of json
