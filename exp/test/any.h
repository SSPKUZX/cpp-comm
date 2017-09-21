#include <cstdlib>
#include <type_traits>

#include "demangle.h"

namespace utl{
	/* *
	 * this class could be constructed or assigned from any type
	 * and converted to its real type, if not, std::logic_error 
	 * gets threw in conversion
	 * */
	class Any{
		public:	
			Any() : m_ptr(nullptr){}
			// support any type T construction and assignment
			template<class T, class RawT= typename std::remove_reference<T>::type >
			Any( T&& val) {
				m_ptr = malloc(sizeof(RawT));
				new (m_ptr) RawT(std::forward<T>(val));
			}
			template<class T>
			Any& operator=( T&& val) {
				this->~Any();
				new (this) Any(std::forward<T>(val));
				return *this;
			}
	
			// only movable
			Any(Any && rhs) : Any() { std::swap(m_ptr, rhs.m_ptr); };
			Any& operator=( Any && rhs){
				if( this != &rhs ){	
					new (this) Any(std::move(rhs));
				}
				return *this;
			}
			// copy is forbidden b' the real type T is uncertain here
			// simple memcpy would rise unknown problem
			Any(Any const&) = delete;
			Any& operator=( Any const&) = delete;
	
			~Any(){
				free(m_ptr); // if it's null, nothing happens	
				m_ptr = nullptr; // avoid second destruction
			}
	
			// when signature is "operator U()", only value conversion is supported 
			// only following signature makes reference/const_refer/value(through copy) conversions possible 
			template<class U>
			operator U&(){
				auto u_ptr = reinterpret_cast<U*>(m_ptr);	
				if( u_ptr){
					return *u_ptr;	
				}else{
					throw std::logic_error(std::string("type=")+ utl::demangle<U>() + " can not be converted from Any.");
				}	
			}
	
		private:
			void* m_ptr;
	};

}
