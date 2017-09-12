#include <iostream>
#include <chrono>

int main(){
	std::cout <<  std::chrono::steady_clock::now().time_since_epoch().count() << '\n';
	std::cout <<  std::chrono::system_clock::now().time_since_epoch().count() << '\n';
	return 0;
}
