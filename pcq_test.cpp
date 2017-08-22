#include "ProducerConsumerQueue.h"
#include <thread>
#include <iostream>

int main(int argc, char* argv[])
{
	comm::ProducerConsumerQueue<uint32_t>	queue(10);

	std::thread consumer([&queue]()
					{ 
						std::this_thread::sleep_for( std::chrono::seconds(1) );
						uint32_t value;
						while( queue.read(value) )
							std::cout << "consumed value=" << value << std::endl;
					}
					);

	for( uint32_t i = 0; i < 10; ++i) queue.write(i);
	
	consumer.join();

	return 0;
}
