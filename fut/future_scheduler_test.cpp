#include "future_scheduler.h"
#include "cmf/cmf.h"
#include "future.h"

#include <iostream>

using namespace cmf;
using namespace utl;

class NetServer final : public AsyncNOffice{
	public:
		NetServer() : AsyncNOffice(3), m_future_scheduler(&this->m_queue_messages){
			// system will bind corresponding system-defined recipients 
			// for  system-defined messages, so no need for user to specify
			// customized recipients
			bind<FunctionMessage>();  
			bind(&NetServer::displayFuture, this, _1);
		}

	private:
		void displayFuture(int const&){
			auto fut = MakeReadyFuture(3);		
			fut.Then(&m_future_scheduler, [this](int const& three){ 
				std::cout << "future gets integrated into cmf with message=" << three << '\n'; 
				std::this_thread::sleep_for(sec(3));
				std::cout << "future gets alive after sleep\n"; 
			}).Then([this](){
				std::cout << "should be printed\n";
				this->m_poster(make_message<CmfStop>()->AriseAfter(sec(0)));
			}).OnTimeout( ms(3200), [this](){  // the scheduler here should not be the one
				// specified in $fut
				std::cout << "future timeouted\n";
			}, &m_future_scheduler);

		}

	private:
		FutureScheduler	m_future_scheduler;
};

class System final : public HeadOffice{
	public:
		System() {
			bindOffice(std::make_shared<NetServer>());
		}
		~System(){}
};

int main(){
	System system;
	std::cout << "begin to run\n";
	system(make_message<int>(3)); // trigger the system
	system.Run();

	return 0;
}
