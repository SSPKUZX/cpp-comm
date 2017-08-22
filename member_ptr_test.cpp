#include "member_ptr.h"

#include <iostream>
#include <cassert>

class Base{
	public:
		virtual ~Base(){}
		virtual void Say() 
		{
			std::cout << "Base says...\n";	
		}
};

struct Child : public Base
{
	public:
		Child( std::string const& name) : m_name(name){}

		virtual void Say() override final
		{
			std::cout << "child says..." << m_name << '\n';	
		}

	private:
		std::string m_name;
};

int main()
{
	using namespace cmn;	
	
	member_ptr<Base> base;
	assert(!base);

	base = make_member<Base>();
	assert(base);

	base->Say();
	base = make_member<Child>("zx");
	base->Say();

	auto base2 = make_member<Child>("lyz");
	base2->Say();

	return 0;
}
