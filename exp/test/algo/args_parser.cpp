#include <string>
#include <unordered_map>

#include <iostream>

namespace yard
{
	namespace args
	{
		using namespace std;
	
		unordered_map<string, string> parse( int argc, char* argv[])
		{
			unordered_map<string, string>	arg_pairs;

			string param, key, value;
			size_t equal_pos;
			for( int idx = 1; idx <  argc; ++idx )
			{
				param		= argv[idx];	
				equal_pos	= param.find_first_of('=');	
				key			= param.substr(1, equal_pos-1);
				value		= param.substr(equal_pos+1);
				arg_pairs.emplace( key,value) ; 
			}

			return arg_pairs;
		}
	}
}

using namespace yard::args;

int main( int argc, char* argv[])
{
	auto arg_pairs = parse( argc, argv);
	for( auto& p : arg_pairs )
	{
		std::cout << "key=" << p.first << "\tvalue=" << p.second << '\n';	
	}
	std::cout << '\n';

	return 0;
}
