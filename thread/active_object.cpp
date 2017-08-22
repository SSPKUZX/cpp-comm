#include <future>
#include <functional>
#include <iostream>

#include "../arch/csp/sync_queue.h"

class Active
{
	public:
		using Message = std::function<void()>;
	
		void Send( Message const& msg){ m_msg_queue.Push( msg ); }		

		Active( Active const&)				= delete;
		Active&	operator= ( Active const&)	= delete;

		Active() : m_done( false), m_asyncer( std::async( [this]{ this->Run(); })	){}
		// send the last msg to stop the asyncer
		~Active() { Send( [this]{ this->m_done = true; });}

	private:
		void Run()
		{
			while( !m_done) { m_msg_queue.Take()(); }	
		}

		std::atomic<bool>			m_done;
		comm::SyncQueue<Message>	m_msg_queue;
		// keep destruction order
		std::future<void>			m_asyncer;
};

class Backgrounder
{
	public:
		void Print( std::string const& newpaper)
		{
			m_active.Send( [&newpaper](){ std::cout << "newpaper=[" << newpaper << "] got printed\n"; } );	
		}

	private:
		// private data first
		// Printer& m_printer;
		// keep destuction order
		Active	m_active;
};

int main()
{
	Backgrounder bg;
	bg.Print("hail, trump");
}
