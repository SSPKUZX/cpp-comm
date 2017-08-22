#include <cstdlib>
#include <unistd.h>
#include <iostream>

int main(int argc, char* argv[])
{
	putenv("mrv2_http_port=8030");

	auto pid = fork();
	if( pid < 0)
			std::cout << "failed in creating sub process\n";
	else if( pid > 0)
			std::cout << "succ in creating sub process with pid=" << pid << "\n";
	else
	{
		auto val = getenv("mrv2_http_port");
		if( nullptr == val)
			std::cout << "can't retrievel env variable in sub process\n";
		else
			std::cout << "retrievel env variable val=" << val << "\n";
	}

	return 0;
}
