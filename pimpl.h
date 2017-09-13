#pragma once

#include "mem_ptr.h"

namespace utl 
{
	template<class Implementor>
	class Pimpl
	{
		template<class Tp>
		friend void swap( Pimpl<Tp>& lhs, Pimpl<Tp>& rhs);

		// Pimpl's members are only accessible to its subclasses
		protected:
			// this only constructor guarantees a non-null implementor pointer
			template<class... Args>
			Pimpl( Args&&... args) : m_impl(new Implementor(std::forward<Args>(args)... )){}; 

			void swap( Pimpl& x) 
			{
				using std::swap;	
				swap(m_impl, x.m_impl);
			}
/*			// if not declare the destructor here, a default 
			// one will be generated which needs a complete 
			// type of PimplImpl because of unique_ptr
			~Pimpl();
			//the default move  needs complete type 
			//of PimplImpl b' it calls desctructor on param
			Pimpl( Pimpl&&);
			Pimpl& operator=( Pimpl&&);


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
*/
		// make it protected to be accessible to $biz.h
		protected:

/*			~Pimpl() { delete m_impl; m_impl = nullptr; }

			Pimpl( Pimpl const& rhs) : m_impl( new Implementor(*rhs.m_impl) ){}

			// work both for Pimpl&& and Pimpl const&
			Pimpl& operator= ( Pimpl rhs)
			{
				swap(*this, rhs);	
			}

			Pimpl( Pimpl&& rhs) : Pimpl()
			{ 
				swap(*this, rhs);
			}
*/
			// MemPtr is used simply the above problems
			MemPtr<Implementor>	 m_impl;
	};

	template<class Tp>
	void swap( Pimpl<Tp>& lhs, Pimpl<Tp>& rhs)
	{
		using std::swap;	
		swap(lhs.m_impl, rhs.m_impl);
	}
}

