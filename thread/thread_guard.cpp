#include <thread>
#include <iostream>


class ThreadGuard final
{
	public:
		using Action = void (std::thread::*)();
		// it's intuitive to put std::thread as the first parameter
		// and syntactically right to keep data member order initialization
		ThreadGuard( std::thread&& t, Action act) : m_action(act), m_thread( std::move(t) ) {}
		// between 'joinable' and '*act', there is no race condition in destructor
		// [ only 'join'&'detach'&'std::move' can make a joinable thread unjoinable]
		~ThreadGuard(){ if( m_thread.joinable() ) (m_thread.*m_action)(); }

		//support moving
		ThreadGuard( ThreadGuard&&) = default;
		ThreadGuard& operator=( ThreadGuard&&) = default;

		// access
		std::thread& get() { return m_thread; }
		operator std::thread& () { return m_thread; }

	private:
		Action		m_action;
		// put std::thread field at last because it will start on initialization,
		// it's better that all other fields get initialized before. 
		std::thread	m_thread; 
};


int main()
{
	// a joinable thread is a thread blocked or to run;	
	// the program terminated if a joinable thread's
	//  destructor is invoked
	// an unjoinable thread may be
	//		1>default constructed thread;
	//		2>moved from thread
	//		3>detached or ofter-join thread
	// join-on-destruction leads to difficult-to-debug performance anomalies
	// detach-on-destruction results in difficult-to-debug undefined behaviors
	
	std::thread runner([](){ std::this_thread::sleep_for(std::chrono::milliseconds(500)); std::cout << "sleep in asyncer\n"; });	
	ThreadGuard  tg( std::move(runner), &std::thread::join);  //&std::thread::detach

	return 0;
}
