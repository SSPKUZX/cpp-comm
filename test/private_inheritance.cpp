/*
 * generally, we would prefer aggregation to non-public inheritance.
 * then when to use non-public inheritance 
 *	1) overriding virtual functions, especially for abstract b' we 
 *		can't initialize it in aggragation;
 *	2) we need protected members( constructors, functions, fields);
 *	3) must init base object first or destroy it after in some critical
 *		situation;
 *	4) using empty base class optimization to gain space advantages; 
 *	5) 
 *
 * */

#include <iostream>

class Base
{
	public:
		// both non-const and const Base object can call this
		virtual void Say() const
		{
			std::cout << __PRETTY_FUNCTION__ << '\n';	
		}

		// non-const object can't call this
/*		virtual void Say() 
		{
			std::cout << __PRETTY_FUNCTION__ << '\n';	
		}
*/		

	protected:
		int m_val;

//	private:
		int k() { return 0;}
};

/*
 * private inheritance should be used only when
 * subclass wants to access protected members or
 * override virtual interfaces
 * generally, composition or aggregation is preferred
 * waring: do not use public inheritance just to
 * reuse code
 * */
class Child : private Base
{
	public:
		virtual void Say() const override
		{
			Base::Say();
			std::cout  << m_val << " revealed in " << __PRETTY_FUNCTION__ << '\n';	
		}

		using Base::k;
};

int main()
{
	Base base;
	base.Say();
	
	Base const& cbase = base;
	cbase.Say();

	Child child;
	child.Say();

	return 0;
}
