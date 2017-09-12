#include<atomic>
#include<iostream>

class UDT
{
	public:
		UDT( int val) : m_iField( val ){}

		bool operator==( UDT const& rhs)
		{
			return true;	
		}

	private:
		uint32_t	m_iField;
};

int main( int argc, char* argv[])
{
	// test point 1 : verify customized operator==
	std::cout << "does 3 equal to 4 ? =" << ( UDT(3) == UDT(4) ? "yes" : "no" )  << std::endl;

	// test point 2 : verify which will be used in compare_exchange_strong : memcmp or operator==
	std::atomic<UDT> atom_1(UDT(1));
	UDT udt_2(2);	
	std::cout << ( atom_1.compare_exchange_strong( udt_2, udt_2 ) ? "operator==" :  "memcmp" )<< " got used in compare_exchange_strong\n";

	// for compare_exchange_strong
	// if as expected, store the value and return true
	// if not, refresh the pass-in 'expected_value' as the real one, and return false;
	std::atomic<int> original(10);
	int expected = 10, new_value = 22;
	original.compare_exchange_strong( expected, new_value );

	//
	// for compare_exchange_weak
	// if as expected, try store; if succ, return true; if fail, return false;
	// if not, refresh ~ and return false
	int expected2 = expected;
	while( !original.compare_exchange_weak( expected2, new_value) && expected == expected2 );
	return 0;
}
