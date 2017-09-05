#include "future.h"

#include <cassert>
#include <iostream>
#include <unistd.h>


using namespace utl;
using namespace std;

int main(){
	// tc 1 : f1 is not ready, then f2 without scheduler
	// tc 2 : f1 is ready, then f2 without scheduler
	//---> f2 will be invoked after f1 without scheduler
	auto f0 = MakeReadyFuture(10);	
	f0.Then( [](int k){
				cout << "before f1's sleep with k=" << k << '\n';	
				sleep(1);
				cout << "after f1's sleep with k=" << k << '\n';	
			}).Then([](){
				cout << "before f2's sleep\n";	
				sleep(2);
				cout << "after f2's sleep\n";	
				return MakeReadyFuture(std::string("yes, string in future")).Then(
					[](std::string const& msg){
						cout << "see what's coming->" << msg << '\n';	
						return std::string("non-ready-future");
					});//std::string("yes,string in non-future");/* */
			}).Then([](std::string const& msg){
				cout << "see what's coming->" << msg << '\n';	
			});

	// considering scheduler
	
	// tc 3 : trigger timeout ( need a scheduler)
/*
	auto f= [](int k){
				cout << "before f1's sleep with k=" << k << '\n';	
				sleep(1);
				cout << "after f1's sleep with k=" << k << '\n';	
		};
	using Void = result_of_t<decltype(f)(int)>;
*/
	return 0;
}
