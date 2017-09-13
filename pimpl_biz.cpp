#include "pimpl_biz.h"

#include <iostream>

class BizImpl
{
	public:
		BizImpl() : m_int(0)
		{
			std::cout << __PRETTY_FUNCTION__ << " gets called\n";	
		}
		void DoWork()
		{
			std::cout << "doing work at " << __PRETTY_FUNCTION__  << " with member=" << m_int << '\n';	
		}
	
	private:
		int m_int;
};


//Biz::Biz() : m_impl( new BizImpl() ){ }//	= default;
//Biz::~Biz() = default;

void Biz::DoWork()
{
	m_impl->DoWork();
}


int main()
{
	Biz biz;
//	biz.DoWork();

	return 0;
}

