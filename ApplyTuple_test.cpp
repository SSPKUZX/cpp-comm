#include "ApplyTuple.h"
#include <iostream>

template< size_t... Is>
struct IndexSequence{};

template< size_t N, size_t... Is>
struct MakeIndexSequence : MakeIndexSequence< N-1, N-1, Is...>{};

template< size_t... Is>
struct MakeIndexSequence<0, Is...> : IndexSequence< Is...>{};

int func(int v1, double v2)
{ return 0;}

int main(int argc, char* argv[])
{
	using namespace folly;		
	std::cout << "applyTuple result: " << applyTuple( func, std::make_tuple(1, 9.0) ) << std::endl;
	return 0;
}
