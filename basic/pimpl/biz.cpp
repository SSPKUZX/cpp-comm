#include "biz.h"

#include <iostream>

class BizImpl
{
	public:
		void DoWork()
		{
			std::cout << "doing work at " << __PRETTY_FUNCTION__ <<	'\n';
		}
};

Biz::Biz() = default; 

void Biz::DoWork()
{
	m_impl->DoWork();
}
