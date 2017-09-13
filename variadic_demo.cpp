#include "variadic.h"
#include "demangle.h"

#include <cassert>
#include <typeindex>
#include <vector>
#include <iostream>

using namespace utl;
using namespace std;


template<class... Types>
std::vector< std::type_index> GetTypeIndexes()
{
	std::vector< std::type_index> indexes;
	vtypeinfos<Types...>([&indexes]( std::type_info const& tinfo){
				indexes.push_back(tinfo);	
			});	

	return std::move(indexes);
}

template<class... Types>
std::vector< std::string> GetTypeNames()
{
	std::vector< std::string> names;
	vtypeinfos<Types...>([&names]( std::type_info const& tinfo){
				names.push_back(demangle(tinfo));	
			});	

	return std::move(names);
}

struct base{};
struct derived1 : public base{};
struct derived2 : public base{};

template<class T>
class InstancesHook
{
	public:	
		template<class BaseType, class... Args>
		void operator()(std::vector< shared_ptr<BaseType>>& result, Args&&... args )
		{
			result.push_back( std::make_shared<T>(std::forward<Args>(args)...) );
		}
};

template< class BaseType, class... Types, class... Args >
std::vector< shared_ptr<BaseType>> CreateInstances( Args&&... args)
{
	std::vector< shared_ptr<BaseType>>	result;
	vtypes<InstancesHook, Types...>()( result, std::forward<Args>(args)... );
	return result;
}

int main()
{
	std::cout << "there are " << GetTypeIndexes<int,long,char>().size() << " typeindexes in vector\n";
	for(auto&& name : GetTypeNames<derived1, derived2>()){
		std::cout << name << '\t';	
	}
	std::cout << '\n';	
	auto instances = CreateInstances< base, derived1, derived2>();
	std::cout << "there are " << instances.size() << " subclasses in vector\n";

	return 0;
}
