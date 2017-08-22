#pragma once

#include "member_ptr.h"

class Biz 
{
	public:
		// 1> Biz's constructors and destructors must not be inlined or defaulted
		// b' that requires complete type of BizImpl
		// 2> constructor args should be used to init BizImpl
		Biz();
		~Biz();

		void DoWork();

	private:
		// the implementor class must be forward declared 
		class BizImpl;
		// does unique_ptr work here ???
		cmn::member_ptr<BizImpl>	m_impl;
};
