#pragma once

#include <utility>

namespace utl 
{
	/*
	 * support deep copy for pointer member
	 * for a simple example, refer to pimpl.h
	 * */
	template<class Tp>
	class member_ptr
	{

		public:
			// member type
			using element_type			= Tp;	// same as shared_ptr|unique_ptr|weak_ptr
			using reference				= Tp&;
			using const_reference		= Tp const&;
			using pointer				= Tp*;
			using const_pointer			= Tp const*;
	
			template<class _Tp>
			friend void swap( member_ptr<_Tp>& lhs, member_ptr<_Tp>& rhs);

		public:
			// constructors
			explicit member_ptr( pointer p = nullptr) : m_ptr(p) {}
			template<class U>
			member_ptr( member_ptr<U> x) : m_ptr(x.release()) {} 

			// destructor
			~member_ptr() { delete m_ptr; m_ptr = nullptr; }

			// rule of four
			member_ptr( member_ptr const& rhs) : m_ptr( new Tp(*rhs.m_ptr) ){}
			member_ptr( member_ptr&& rhs) : member_ptr() { swap(rhs); }
			// work both for member_ptr&& and member_ptr const&
			member_ptr& operator= ( member_ptr rhs){ swap(rhs); return *this; }
			// specialize for nullptr_t
			member_ptr& operator=( std::nullptr_t p)
			{ reset(p); return *this;} 

			template<class U>
			member_ptr& operator= (member_ptr<U> x)
			{ reset(x.release()); return *this; }
			

			// pointer like function
			pointer operator->() { return m_ptr;}
			const_pointer operator->() const{ return m_ptr;}
			reference operator*() { return *m_ptr;}
			const_reference operator*() const{ return *m_ptr;}

			// pointer-related
			pointer get() const noexcept { return m_ptr;}
			void reset( pointer p = nullptr) noexcept 
			{ this->~member_ptr(); m_ptr = p; }
			// release ownership of m_ptr to the caller
			pointer release() noexcept
			{ 
				pointer p= m_ptr; 
				m_ptr = nullptr; 
				return p; 
			}

			// auxiliary utilities
			explicit operator bool() const noexcept 
			{ return nullptr != m_ptr; }
			void swap( member_ptr& x) noexcept
			{
				std::swap(m_ptr, x.m_ptr);	
			}

		private:	
			pointer	m_ptr;
	};

	template<class Tp>
	void swap( member_ptr<Tp>& lhs, member_ptr<Tp>& rhs)
	{
		std::swap(lhs.m_ptr, rhs.m_ptr);
	}

	template<class Tp, class... Args>
	member_ptr<Tp> make_member( Args&&... args)
	{
		return member_ptr<Tp>( new Tp(std::forward<Args>(args)...) );	
	}
}
