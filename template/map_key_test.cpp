#include <map>
#include <iostream>

struct KeyType
{
	int id;
};

template<>
struct std::less< KeyType >
{
	bool operator() ( const KeyType& lhs, const KeyType& rhs) const 
	{
		return lhs.id < rhs.id;
	}
};

int main()
{
	std::map< KeyType, int > k2intMap;
	
	KeyType _key;
	_key.id = 102;
	if( k2intMap.end() != k2intMap.find( _key) )
	{
		std::cout<< " yes, found!!!\n";
	}
	else
	{
		std::cout<< " no, not found!!!\n";
	}

	return 0;
}
