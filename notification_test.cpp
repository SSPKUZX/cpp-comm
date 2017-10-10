#include "notification.h"

#include <cassert>
#include <future>


int main(){
	using namespace utl;

	Notification n;
	auto fi2 = std::async(std::launch::async,[&n](){ assert(not n);n.Wait(std::chrono::milliseconds(200)); assert(not n);return 5;});
	auto fi = std::async(std::launch::async,[&n](){ assert(not n);n.Wait(); assert(n);return 3;});
	assert(not n);
	std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	n();
	assert(n);
	assert(fi.get()==3);
	assert(fi2.get()==5);

	return 0;
}
