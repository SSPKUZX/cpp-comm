#include "ticked_scheduler.h"

#include <iostream>
#include <thread>

using namespace wxg;

int main(int argc, char* argv[]){
	
	TickedScheduler scheduler(100);

	scheduler.ScheduleAfter<-1>( DurationMs(3), [](){ std::cout << "saying hello\n";});
	scheduler.ScheduleAfter( DurationMs(10), [&scheduler](){ scheduler.Stop(); std::cout << "stopped ticking\n";});

	// this must be invoked after 'ScheduleAfter|At'
	scheduler();

	return 0;
}
