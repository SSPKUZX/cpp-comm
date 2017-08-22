#include "unistd.h"
#include <iostream>

int main()
{
	int ret = execlp( "/bin/sh", "sh", "-c", "ls -lh", NULL );
	if( 0 == ret )
		std::cout << "succeeded in executing shell\n";
	else
		std::cout << "failed in executing shell\n";

	return 0;
}
