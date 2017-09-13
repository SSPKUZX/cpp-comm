#include "port_picker.h"
#include "retry.h"
#include "bind.h"
#include <iostream>


int main( int argc, char* argv[])
{
	using namespace utl;

	for( int i = 0; i < 1; ++i )
	{
		// port_picker is used for supporting template function like retry
		auto port= retry( std::bind(port_picker(), ranged, 31050, 31099), is_valid_port );
		std::cout << std::boolalpha << is_valid_port( port ) << std::endl;

		port = pick_port( ranged, 31050, 31099 );
		std::cout << std::boolalpha <<is_valid_port( port ) << std::endl;

		port = pick_port( assigned, 8030 );
		std::cout << std::boolalpha <<is_valid_port( port ) << std::endl;

		// zero means randomed
		port = pick_port( assigned, RANDOM_PORT );
		std::cout << std::boolalpha <<is_valid_port( port ) << std::endl;

		port = pick_port( [](){ return 8090;} );
		std::cout << std::boolalpha <<is_valid_port( port ) << std::endl;
	}

	return 0;
}

