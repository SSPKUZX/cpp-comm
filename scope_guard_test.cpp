#include "scope_guard.h"

#include <cassert>
#include <stdexcept>

int main(){
	using namespace utl;

	int k=0;
	// case1: SCOPE_EXIT
	{
		SCOPE_EXIT{
			k=9;	
		};	
	}	
	assert(k==9);
	try{
		{
			SCOPE_EXIT{
				k=10;	
			};	
			throw std::logic_error("exception in scope_exit");
		}	
	}catch(...){}
	assert(k==10);

	// case2: dismiss 
	{
		auto guard = MakeGuard([&k](){
			k =8;				
		});
		// no exceptions
		guard.Dismiss();
	}	
	assert(k==10);
	try{
		{
			auto guard = MakeGuard([&k](){
				k =8;				
			});
			// exception occurs
			throw std::logic_error("exception in dismissing guard");
			guard.Dismiss();
		}	
	}catch(...){}
	assert(k==8);

	// case3: SCOPE_SUCC
	{
		SCOPE_SUCC{
			k=6;	
		};	
	}	
	assert(k==6);
	try{
		{
			SCOPE_SUCC{
				k=7;	
			};	
			throw std::logic_error("exception in scope_succ");
		}	
	}catch(...){}
	assert(k==6);
	
	// case4: SCOPE_FAIL
	{
		SCOPE_FAIL{
			k=2;	
		};	
	}	
	assert(k==6);
	try{
		{
			SCOPE_FAIL{
				k=2;	
			};	
			throw std::logic_error("exception in scope_fail");
		}	
	} catch(...){}
	assert(k==2);

	// compilation error
	//auto sg_ptr = new ScopeGuard(nullptr);
	//auto esg_ptr = new ExceptionalScopeGuard<true>(nullptr);

	return 0;
}
