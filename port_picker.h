#pragma once

#include <cstdlib>
#include <utility>
#include <algorithm>


namespace utl 
{
	// try socket connecting to port
	uint32_t try_port( uint32_t const port );
	
	// try a port in the picking way, and return it
	template< class PickWay, class... Args>
	inline uint32_t pick_port( PickWay&& way, Args&&... args) 
	{
		auto port = std::forward<PickWay>(way)( std::forward<Args>(args)... );
		return try_port( port );
	}
	
	// supporting as template parameter
	class port_picker
	{
		public:
			template< class PickWay, class... Args>
			inline uint32_t operator()(PickWay&& way, Args&&... args){
				return pick_port(std::forward<PickWay>(way), std::forward<Args>(args)...);	
			}
	};
	
	inline uint32_t ranged( uint32_t const from, uint32_t const to) {
		return std::min(from, to) + rand() % ( std::abs((int)(to - from)) + 1);
	}

	// to pick a random port, call ' assigned( RANDOM_PORT) '
	static const uint32_t RANDOM_PORT = 0;
	inline uint32_t assigned( uint32_t const port){
		return port;
	}
	
	inline bool is_valid_port( uint32_t const port){
		return port > 0 && port < 65536;
	}
	
} // end of utl 

