#include "call.h" 
#include <iostream>


// below are test case for call_each( prepare for jsonize)
class SumInvoker
{
	public:
		SumInvoker() : m_iResult(0) {}
		template< class _Arg>
		void operator()( _Arg const& arg )
		{
			m_iResult += arg;
		}

		int const& get()
		{
			return m_iResult;
		}

	private:
		int m_iResult;
};

template< class _Args>
void sum( _Args const& args, int& result){ result = 0; }

class type
{
	public:
		int a,b;
};

template<>
void sum<type>( type const& args, int& result)
{
	SumInvoker caller;
	comm::call_each( caller, args.a, args.b);
	result = caller.get();
}

void say( std::string const& key)
{
	std::cout << "saying key=" << key << std::endl;
}

int get_port( std::string const& msg) 
{
	std::cout << "saying msg=" << msg << "\n"; 
	return 3; 
}

int main(int argc, char* argv[])
{
	// test case for call_once_per_key
	std::string key1("key1");
	const std::string key2("key1");
	const std::string& key3 = key2;
	std::string& key4 = key1;
	std::string word = "hello, call_once_per_key";
	using namespace comm;
	// below eight examples are the same
	// which means const|reference|value KeyType are the same
	// but in order to guarantee to call the same 'call_once_pey_key'
	// all args types, including keyType&Callable&Args...， must
	// stay the same
	std::cout << "\nbelow are test result for call_once_per_key\n";
	call_once_per_key<std::string>( key1, say, word);
	call_once_per_key( key1, say, word);
	std::cout << std::endl;
	call_once_per_key<std::string>( key2, say, word);
	call_once_per_key( key2, say, word);
	std::cout << std::endl;
	call_once_per_key<std::string>( key3, say, word);
	call_once_per_key( key3, say, word);
	std::cout << std::endl;
	call_once_per_key<std::string>( key4, say, word);
	call_once_per_key( key4, say, word);

	// test case for call_once_only
	std::cout << "\nbelow are test result for call_once_only\n";
	auto word2 = "hello, call_once_only";
	call_once_only( say, word2);
	call_once_only( say, word2);
	call_once_only( say, word2);
	
	// test case for call_if
	std::cout << "\nbelow are test result for call_if\n";
	call_if( true, say, "hello, call_if#bool");
	call_if( [](){return true;}, say, "hello, call_if#predicate#without return");
	auto sp_port = call_if( [](){return true;}, get_port, "hello, call_if#predicate#with return");
	if(sp_port) std::cout << "succ in calling call_if with sp port=" << *sp_port << std::endl;

	// test case for call_until
	std::cout << "\nbelow are test result for call_until#predicate()\n";
	call_until( [](){ static uint32_t times=0;return ++times>=1;}, say, "hello, call_until#predicate()");
	std::cout << "\nbelow are test result for call_until#predicate(R)\n";
	call_until( [](int const ret){ static uint32_t times=0;return ret!=3 || ++times>=3;}, get_port, "call_until#predicate(R)");

	// test case for call_multiple
	std::cout << "\nbelow are test result for call_multiple\n";
	auto word3 = "hello, call_multiple";
	call_multiple( 3, say, word3);// call_each test

	// test case for call_each
	std::cout << "\nbelow are test result for call_each\n";
	type tp;
	tp.a	= 1;
	tp.b	= 2;
	std::cout << "tp.a=" << tp.a << "\ttp.b=" << tp.b << std::endl;
	int result;
	sum( tp, result);
	std::cout << "sum of tp is " << result << std::endl;
	return 0;
}
