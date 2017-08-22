#include "pimpl_biz.h"

#include <iostream>

class BizImpl
{
	public:
		BizImpl()
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


Biz::Biz() : m_impl( new BizImpl() ){ }//	= default;
Biz::~Biz() = default;

void Biz::DoWork()
{
	m_impl->DoWork();
}

/*
int main()
{
	BizImpl* pBiz = nullptr;
	// this would work because 'DoWork' doesn't use member data
	// it's an absolute address
	pBiz->DoWork();

	return 0;
}
*/
