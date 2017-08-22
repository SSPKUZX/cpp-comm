#pragma once

#include <utility>
#include "call.h"

namespace usual
{
	using namespace comm;

	// try socket connecting to port
	uint32_t try_port( uint32_t const port );

	template< class PickWay, class... Args>
	uint32_t pick_port( PickWay&& way, Args&&... args) 
	{
		auto port = call_any( std::forward<PickWay>(way), std::forward<Args>(args)... );
		return try_port( port );
	}
	
	template< class PickWay, class... Args>
	uint32_t port_picker::operator()(PickWay&& way, Args&&... args)
	{
		return pick_port(std::forward<PickWay>(way), std::forward<Args>(args)...);	
	}
	
} // end of usual
