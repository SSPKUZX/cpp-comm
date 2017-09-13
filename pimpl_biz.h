#pragma once

#include <memory>
#include "member_ptr.h"

// the implementor class must be forward declared 
class BizImpl;

class Biz 
{
	public:
		// 1> Biz's constructors and destructors must not be inlined or defaulted
		// b' that requires complete type of BizImpl
		// 2> constructor args should be used to init BizImpl
//		Biz();
//		~Biz();

		void DoWork();

	private:
	//	utl::member_ptr<BizImpl>	m_impl;
	// does unique_ptr work here --> uptr needs that BizImpl should be complete 
	// be' default ~Biz() needs it, but actually didn't ???? 
		std::unique_ptr<BizImpl>	m_impl;
	//	std::shared_ptr<BizImpl>	m_impl;
};
