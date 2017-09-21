#include "demangle.h"

#include <cxxabi.h>
#include <cstdlib>

namespace utl 
{
	template<>
	std::string demangle( std::type_info const& ti) {
	  int status;
	  // malloc() memory for the demangled type name
	  char* demangled = abi::__cxa_demangle(ti.name(), nullptr, nullptr, &status);
	  std::string dname;
	  if (status != 0) {
		  dname = ti.name();
	  } else {
		  dname = demangled;
		  std::free(demangled);
	  }
		return std::move(dname);
	}
}
