#include "future_scheduler.h"
#include "cmf/cmf.h"
#include "future/future.h"

using namespace cmf;
using namespace utl;

class NetServer final : public AsyncOffice{
	public:
		NetServer(){
			bind<FutureMessage>([this](FutureMessage const& msg){
				std::cout << "before FutureMessage gets processed\n";
				msg();
				std::cout << "after FutureMessage gets processed\n";
				this->m_poster.emplace<CmfStop>();
			});	
		}
		~NetServer(){}

};

class System final : public HeadOffice{
	public:
		System() : m_future_scheduler(&this->m_queue_messages){
			bind<int>(&System::displayFuture, this);
			bindOffice(std::make_shared<NetServer>());
		}
		~System(){}

	private:
		void displayFuture(int const&){
			auto fut = MakeReadyFuture(3);		
			fut.Then([](int const& three){ 
				std::cout << "future gets integrated into cmf with message=" << three << '\n'; },
				&m_future_scheduler
			);
		}

	private:
		FutureScheduler	m_future_scheduler;
};

int main(){
	System system;
	system(make_message<int>(3)); // trigger the system
	system.Run();

	return 0;
}