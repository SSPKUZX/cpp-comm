/**
 * invoke function with its parameters 
 * rather than binding them first 
 */
 
#include "invoke.h"
#include <cassert>
#include <functional>
 
namespace utl{
}

int f(){ return 4; }

struct F{
	int say(int){ return 5;} 
	int tell(int) const{ return 5;} 
	int talk(int) &&{ return 6;} 
//	int talk(int) &{ return 7;} 
//	int talk(int) const&{ return 8;} 
};

int main(){
	using namespace utl;

	// function like
	assert(4==Invoke(&f));

	// member function pointer
	F func;
	assert(5==Invoke(&F::say,func,3));
	assert(6==Invoke(&F::talk,F(),3));
//  following can't infer F::talk if it has overloads
//	assert(6==std::mem_fn(&F::talk)(F(),3));
	assert(5==Invoke(&F::say,F(),3));
	assert(5==Invoke(&F::say,&func,3));

	// member function pointer
	F const cfunc;
	assert(5==Invoke(&F::tell,&cfunc,3));
	assert(5==Invoke(&F::tell,cfunc,3));
	assert(5==Invoke(&F::tell,func,3));

	return 0;
}
