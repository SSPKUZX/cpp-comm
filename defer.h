#pragma once

#include <functional>

namespace utl 
{

namespace
{

// The defer class for C++11
class Defer
{
	public:
	    Defer() = default; 
	    Defer(Defer&& e) : m_functor(std::move(e.m_functor)) {} 
	    
	    Defer(const Defer& e) = delete;
	    Defer& operator=(const Defer& f) = delete;
	    
	    template <typename F, typename... Args>
	    Defer(F&& f, Args&&... args){
	        m_functor = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
	    }
	    
	    ~Defer() noexcept {
	        if (m_functor)  m_functor();
	    }
	    
	private:
	    std::function<void ()> m_functor;
};
    
} // end namespace

#undef	_CONCAT
#define _CONCAT(a, b) a##b
#define _MAKE_DEFER_HELPER_(line)  ::utl::Defer _CONCAT(utl_defer, line) = [&]()

#undef	UTL_DEFER
#define UTL_DEFER _MAKE_DEFER_HELPER_(__COUNTER__)

} // end namespace utl 


