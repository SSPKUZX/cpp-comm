#include "jsonize.h"
#include <iostream>

class type
{
	public:
		uint32_t age, workdays;
};

template<>
bool json::obj2json<type>( type const& tp, std::string& json )
{
	return ser( json, typeid(type), tp.age, tp.workdays );
}


template<>
bool json::json2obj<type>( std::string const& json, type& tp )
{
	return des( json, typeid(type), tp.age, tp.workdays );
}

int main(int argc, char* argv[])
{
	using namespace json;

	// test for serialization
	reg_type( typeid(type), {"age", "workdays"} );
	// auto meta =std::vector<std::string>({"age__", "workdays__"});
	//reg_type( typeid(type), meta);

	std::string json;	
	type tp={ 2, 3};
	if( obj2json(tp, json) )	
	{
		std::cout << "succ in serialize type to json\n" << json << std::endl;
	}
	else
	{
		std::cout << "fail in serialize type with age=" << tp.age << ", workdays=" << tp.workdays << std::endl;
	}


	// test for deserialization
	if( json2obj(json, tp) )	
	{
		std::cout << "succ in deserialize type to json\ntp.age=" << tp.age << "\ttp.workdays=" << tp.workdays << std::endl;
	}
	else
	{
		std::cout << "fail in deserialize json=" << json << std::endl;
	}
	
	return 0;
}
