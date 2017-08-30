#include "defer.h"

#include <iostream>

int main(){
	{
		UTL_DEFER
		{
			std::cout << "call in defer\n";	
		};	
	}
	std::cout << "call out defer\n";	

	return 0;
}
