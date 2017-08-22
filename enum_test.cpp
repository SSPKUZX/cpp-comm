#include <iostream>

#include "enum.h"
#include <unordered_set>
#include <map>

namespace cmn 
{
	struct Succ{};
	struct Fail{};
	struct NoState{}; // for static compilation test

	using State = Enum<Succ, Fail>;
}

int main()
{
	using namespace cmn;
	using namespace std;

	auto state = State::Of<Fail>();	
	if( state.Is<Fail>()  )
	{
		cout << "current state is " << state << '\n';	
		cout << "current state val is " << (int)state << '\n';	
	}
	state = State::Of<Succ>();
	cout << "current state is " << state.Str() << '\n';	
	cout << "current state val is " << (int)state << '\n';	
//	auto undefined_state = State::of<NoState>();  // this won't compile 
	
	// testing no duplicate class labels
//	Enum<int, long, char, std::string, int>::Of<int>(); 
//	Enum<int, int>::Of<int>(); 
//	Enum<int>::Of<int>(); 

//	Enum<> e = Enum<>::Of<int>();
//	state.Is<NoState>();	

	// testing operator< and hash
	unordered_set<Enum<int,char>> use;
	map<Enum<int,char>, int> me;
	me.emplace(Enum<int,char>::Of<int>(), 2);

	if(state >= State::Of<Succ>())	
		cout << "operator int() worked in arithmetic comparation\n";
		
	return 0;
}
