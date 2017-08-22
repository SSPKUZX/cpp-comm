#include "pimpl.h"

namespace cmn
{
	template<class Implementor>
	template< class... Args>
	Pimpl<Implementor>::Pimpl( Args&&... args) : m_impl(std::make_shared<Implementor>(std::forward<Args>(args)... )){} 

/*	template<class Implementor>
	Pimpl<Implementor>::~Pimpl()					= default;
	template<class Implementor>
	Pimpl<Implementor>::Pimpl( Pimpl&&)				= default;
	template<class Implementor>
	Pimpl<Implementor>& Pimpl<Implementor>::operator=( Pimpl<Implementor>&&)	= default;

	template<class Implementor>
	Pimpl<Implementor>::Pimpl( Pimpl const& rhs) : m_impl( std::make_shared<Implementor>(*rhs.m_impl) ){}

	template<class Implementor>
	Pimpl<Implementor>& Pimpl<Implementor>::operator= ( Pimpl<Implementor> const& rhs)
	{
		*m_impl = *rhs.m_impl;
		return *this;
	}
*/

}
