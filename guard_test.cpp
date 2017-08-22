#include <iostream>

#include "guard.h"

int main()
{
	int k = 9;
	{
		auto ig = cmn::make_guard( k, [](int& val){ ++val; }, [](int& val){++val;} );
	}
	std::cout << "value of k is=" << k << '\n';
}
