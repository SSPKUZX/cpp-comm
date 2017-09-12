#include <future>
#include <vector>
#include <stdexcept>
#include <iostream>

template<class T>
/*
 * ensure that the promise is set before destructed 
 * to avoid future_error and related problems
 * */
class PromiseGuard
{
	public:	
		PromiseGuard( std::promise<T>& p) : m_promise(p){}
		~PromiseGuard()
		{
			/////// if promise is not set, set an exception
			try{
				m_promise.set_exception( std::make_exception_ptr(std::runtime_error("set exception for promise to avoid thread suspension")) );
			}catch(...)
			{
				std::cout << "an result has ever been set to promise before\n";		
			}
		}

		PromiseGuard( PromiseGuard&& guard)				= delete;
		PromiseGuard& operator=( PromiseGuard&& guard)	= delete;
		PromiseGuard( PromiseGuard const&)				= delete;
		PromiseGuard& operator= ( PromiseGuard const&)	= delete;
	
	private:
		std::promise<T>&		m_promise;
};

// after the publisher publishes one event
// an subscriber reacts to it
template<class Event>
class Publisher 
{
	public:
		template<class Subscriber>
		Publisher( Subscriber&& ss) // no member list here to default init m_promise
		{
			m_asyncer = std::async( std::launch::async,
				[&]()
				{	
					try{
						ss(m_promise.get_future().get());	
					}catch(...)
					{
						std::cout << "got exception in " << __PRETTY_FUNCTION__ << '\n';	
					}
				}
			);			
		}

		void Publish(Event const& evt)
		{
			m_promise.set_value(evt);	
		}

	private:
		std::future<void>	m_asyncer;
		//if promise get destructed before asyncer without setting, 
		//exception get throwed to unsuspend the asyncer 
		std::promise<Event>	m_promise; 
};

void notify()
{
	auto subscriber =[]( int const& event)
	{
		try{
			std::cout << "one shot event received =" << event << '\n';	
		}catch(std::exception& e)
		{	
			std::cout << "got exception=" << e.what() << " in getting result from future\n";
		}
	};

	try{
		Publisher<int> subject(subscriber);
		throw std::runtime_error("simulate exception throwing");
		subject.Publish(4);
	}catch(...)
	{
		std::cout << "got exception in " << __PRETTY_FUNCTION__ << '\n';
	}
}


struct type{ int k; };

void one_shot()
{
	std::promise<type> event;

	// asyncer will implicitly join on its destruction
	auto asyncer = std::async(std::launch::async, [&event]()
		{
			try{
				// when promise sets a default null exception_ptr, this 'get' just returns 
				// a default result without throwing 
				auto val = event.get_future().get(); 
				std::cout << "one shot event received with val=" << val.k << '\n';	
			}catch(std::exception& e)
			{	
				std::cout << "got exception=" << e.what() << " in getting result from future\n";
			}
		}
		);
	try{  // if there is any exceptions not caught, program cores.
	  std::cout << "triggering event\n";
	  PromiseGuard<type> pg(event);
	  // simulate exception throwing
	  throw std::runtime_error("simulate exception throwing here");

	  // the set here wont be executed, this is what PromiseGuard works for
	  event.set_value(type{3});
	}
	catch(...)
	{
		std::cout << "catch all exceptions globally\n";	
	}
}

void multiple_shots()
{
	std::promise<int> event;

	auto shared_event = event.get_future().share();
	std::vector< std::future<void> > subscribers(7);
	for( size_t idx = 0; idx < 7; ++idx)
	{
		subscribers.emplace_back( std::async(std::launch::async, 
			[shared_event]()			
			{
				std::cout << "waiting on future\n";
				auto event = shared_event.get();	
				std::cout << "received event:" << event << "\n";
			})	);
	}

	try
	{
		std::cout << "triggering event\n";
		PromiseGuard<int> pg(event);

		//in real situation, exception  may or may not get throwed
		throw std::runtime_error("simulate exception throwing here");

		event.set_value(7);
	}catch(...)
	{
		std::cout << "catch exceptions throwed before setting value for promise\n"; 		
	}
}

int main()
{
	one_shot();
	std::cout << "\n\n";
	multiple_shots();
	std::cout << "\n\n";
	notify();
	return 0;
}

