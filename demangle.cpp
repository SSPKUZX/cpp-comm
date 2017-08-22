#include "Demangle.h"
#include <cxxabi.h>

namespace comm
{
	std::string demangle( std::type_info const& ti) {
	  int status;
	  size_t len = 0;
	  // malloc() memory for the demangled type name
	  char* demangled = abi::__cxa_demangle(ti.name(), nullptr, &len, &status);
	  if (status != 0) {
	    return ti.name();
	  }
	  return demangled;
	}
}
