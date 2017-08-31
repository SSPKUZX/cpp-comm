#pragma once

#include <functional>

namespace utl{

	class FutureMessage final{
		public:	
			FutureMessage(std::function<void()> functor) : m_functor(std::move(functor)){}
			void operator()()const{ m_functor(); }

		private:
			std::function<void()>	m_functor;	
	};	
}
