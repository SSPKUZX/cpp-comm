#include <iostream>
#include "Picture.h"

int main()
{
	char *raw[] = {"Paris ", "in the", "spring"};
	
	Picture p(raw, 3);
	std::cout << p << std::endl;

	Picture q = frame(p);
	std::cout << q << std::endl;

	Picture r = p | q;
	std::cout << frame(r) << std::endl;

	Picture s = q & r;
	std::cout << frame(s) << std::endl;

	Picture v = frame(p | q | r | s);
	Picture h = frame(p & q & r & s);
	std::cout << frame(h | v | h) << std::endl;

	return 0;
}
