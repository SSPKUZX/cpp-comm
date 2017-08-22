#pragma once

#include <cstdint>

namespace usual 
{
	
	// try a port in the picking way, and return it
	template< class PickWay, class... Args>
	uint32_t pick_port( PickWay&& way, Args&&... args); 
	
	// supporting as template parameter
	class port_picker
	{
		public:
			template< class PickWay, class... Args>
			uint32_t operator()(PickWay&& way, Args&&... args);
	};
	
	// return port in [ from, to ] 
	uint32_t ranged( uint32_t const from, uint32_t const to);
	
	// to pick a random port, call ' assigned( RANDOM_PORT) '
	static const uint32_t RANDOM_PORT = 0;
	// return the specified port
	uint32_t assigned( uint32_t const port);
	
	// test if it's a valid port
	bool is_valid_port( uint32_t const port);
	
} // end of usual 

#include "port_picker_inl.h"
