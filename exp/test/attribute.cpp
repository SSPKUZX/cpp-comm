#include <iostream>

//class OldName{};

class NewName{};


__attribute__((visibility("hidden")))
int k() { return 0;}

__attribute__((deprecated("OldName is deprecated, please use NewName")))
//using OldName = NewName;
typedef NewName OldName;

int main()
{
	OldName on;
	return 0;
}
