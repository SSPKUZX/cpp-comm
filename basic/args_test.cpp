#include <iostream>

int main(int argc, char* argv[])
{
	for( int i =0 ;i < argc; ++i )
		std::cout << argv[i] << "\t";
	std::cout << std::endl;

	return 0;
}
