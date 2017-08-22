#pragma once

#include <memory>

namespace cmn
{
	template<class Implementor>
	class Pimpl
	{
		public:
			template<class... Args>
			Pimpl( Args&&... args); 

//			Pimpl( Implementor* impl = nullptr) : m_impl(impl){};
/*			// if not declare the destructor here, a default 
			// one will be generated which needs a complete 
			// type of PimplImpl because of unique_ptr
			~Pimpl();
			//the default move  needs complete type 
			//of PimplImpl b' it calls desctructor on param
			Pimpl( Pimpl&&);
			Pimpl& operator=( Pimpl&&);
*/

			// the default one just shadow copy unique_ptr
			// so customization is needed
		//	Pimpl( Pimpl const&);
		//	Pimpl& operator=( Pimpl const&);


		protected:
			// if using shared_ptr, no need to declare the 'five'
			// because the deleter is not part of the shared_ptr  
			// but part of unique_ptr which needs a complete type
			// in its constructor
			//std::unique_ptr< Implementor>	m_impl;
			std::shared_ptr< Implementor>	m_impl;
	};
}

#include "pimpl_inl.h"
