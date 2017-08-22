#pragma once

#include "mem_ptr.h"

namespace cmn
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

		// make it protected to be accessible to $biz.h
		protected:
			MemPtr<Implementor>	 m_impl;
	};

	template<class Tp>
	void swap( Pimpl<Tp>& lhs, Pimpl<Tp>& rhs)
	{
		using std::swap;	
		swap(lhs.m_impl, rhs.m_impl);
	}
}

