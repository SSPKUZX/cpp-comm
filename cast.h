#pragma once

#include <string>
#include <sstream>

namespace utl{
	template<class T>
	std::string lexical_cast(T const& t){
		static thread_local std::ostringstream oss;	
		oss.str("");
		oss << t;
		return oss.str();
	}


}
