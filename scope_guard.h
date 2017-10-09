#pragma once
/*
 * idea from following articles and folly/ScopeGuard.h
 *   Andrei's and Petru Marginean's CUJ article:
 *     http://drdobbs.com/184403758
 *   and the loki library:
 *     http://loki-lib.sourceforge.net/index.php?n=Idioms.ScopeGuardPointer
 *   and triendl.kj article:
 *     http://www.codeproject.com/KB/cpp/scope_guard.aspx
 * */
#include <functional>

#include "uncaught_exceptions.h"

namespace utl{
	class ScopeGuard{
		public:	
			// support implicit conversion
		//	template<class FuncType>
		//	explicit ScopeGuard(FuncType&& func) noexcept(std::is_nothrow_constructible<std::function<void()>,FuncType>::value)
		//		: m_guard(std::forward<FuncType>(func)), m_dismissed(false){}
			// the compliler will implicitly convert const/FuncType/& to std::function<void(void)> 
			explicit ScopeGuard( std::function<void(void)> func) 
				noexcept(std::is_nothrow_move_constructible<std::function<void()>>::value)  
				: m_guard(std::move(func)){}

			ScopeGuard( ScopeGuard&& other) noexcept(std::is_nothrow_move_constructible<std::function<void()>>::value) 
				: m_guard(std::move(other.m_guard)){}

			void Dismiss() noexcept{
			//	m_dismissed = true;
				m_guard = nullptr;
			}

			// forbid const copy constructor and assignment
			ScopeGuard(ScopeGuard const&)				= delete;
			ScopeGuard& operator=(ScopeGuard const&)	= delete;
			ScopeGuard& operator=(ScopeGuard&&)			= delete;
			// forbid allocation on heap
			void* operator new(std::size_t)				= delete;	

			~ScopeGuard() noexcept{
				if(m_guard){
					// no exception should happen in destructor
					try{
						m_guard();	
					} catch(...){}
				}	
			}

		private:
			std::function<void(void)>	m_guard;
//			bool						m_dismissed;	
	};

	ScopeGuard MakeGuard(std::function<void(void)> func) 
		noexcept(std::is_nothrow_move_constructible<std::function<void()>>::value){
		return ScopeGuard(std::move(func));	
	}

	struct ScopeExit{};
	template<class FuncType>
	ScopeGuard operator+(ScopeExit se, FuncType&& func){
		return ScopeGuard(std::forward<FuncType>(func));	
	}

/*
 * If the executeOnException template parameter is true, the function is
 * executed if a new uncaught exception is present at the end of the scope.
 * If the parameter is false, then the function is executed if no new uncaught
 * exceptions are present at the end of the scope.
 */
	template<bool ExecuteOnException>
	class ExceptionalScopeGuard : private ScopeGuard{
		public:	
			template<class FuncType>
			explicit ExceptionalScopeGuard( FuncType&& func) : ScopeGuard(std::forward<FuncType>(func)),
				m_pre_exception_count(uncaught_exceptions()){}
			
			ExceptionalScopeGuard( ExceptionalScopeGuard&& other) 
				noexcept(std::is_nothrow_move_constructible<std::function<void()>>::value) = default;

			~ExceptionalScopeGuard() noexcept{
				if (ExecuteOnException ^ hasNewUncaughtExceptions() ) {
					this->Dismiss();
				}
			}

		private:
			inline bool hasNewUncaughtExceptions(){
				return uncaught_exceptions() > m_pre_exception_count;	
			}

			const int	m_pre_exception_count;
	};
	
	struct ScopeSucc{};
	template<class FuncType>
	ExceptionalScopeGuard<false> operator+(ScopeSucc ss, FuncType&& func){
		return ExceptionalScopeGuard<false>(std::forward<FuncType>(func));	
	}

	struct ScopeFail{};
	template<class FuncType>
	ExceptionalScopeGuard<true> operator+(ScopeFail sf, FuncType&& func){
		return ExceptionalScopeGuard<true>(std::forward<FuncType>(func));	
	}
} // end of utl

#define SCOPE_EXIT auto	scope_guard_##__COUNTER__ = utl::ScopeExit{} + [&]()
#define SCOPE_SUCC auto exceptioal_scope_guard_##__COUNTER__ = utl::ScopeSucc{} + [&]()
#define SCOPE_FAIL auto exceptioal_scope_guard_##__COUNTER__ = utl::ScopeFail{} + [&]()
