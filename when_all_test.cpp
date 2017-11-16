#include "when_all.h"

#include <iostream>

using namespace utl;

int main(){
	auto fut1 = std::async([](){
				std::this_thread::sleep_for(std::chrono::milliseconds(300));			
				return 4;
			});
	auto fut2 = std::async([](){
				std::this_thread::sleep_for(std::chrono::milliseconds(400));			
				return 5;
			});
	auto fut3 = std::async([](){
				std::this_thread::sleep_for(std::chrono::milliseconds(200));			
				return 8;
			});

	std::vector<int>	results;
	if(false){
		std::vector<std::future<int>> futures;
		futures.push_back(std::move(fut1));
		futures.push_back(std::move(fut2));
		futures.push_back(std::move(fut3));
		results = WhenAll(std::move(futures));
	} else {
		results = WhenAll( fut1, fut2, fut3);
	}
	std::copy(results.begin(), results.end(), std::ostream_iterator<int>(std::cout, "\t") );
	std::cout << std::endl;

	std::promise<int> tuple_pro1;	
	tuple_pro1.set_value(3);
	auto tuple_fut1 = tuple_pro1.get_future();
	std::promise<long> tuple_pro2;	
	tuple_pro2.set_value(5l);
	auto tuple_fut2 = tuple_pro2.get_future();
	auto tup = WhenAll(tuple_fut1, tuple_fut2);
	std::cout << std::get<0>(tup) << '\t' << std::get<1>(tup) << '\n';

	return 0;
}
