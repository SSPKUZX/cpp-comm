/**
 * invoke function with its parameters 
 * rather than binding them first through std::bind or std::mem_fn
 */
#pragma once

#include "traits.h" 
 
namespace utl{
	//functional Type like std::function/operator() type/lambda/function pointer/const function pointer/function reference
	template<class FuncType, class... Args>		
	inline auto Invoke(FuncType&& f, Args&&... args)
		-> typename std::enable_if< not is_member_function_ptr<FuncType>::value, 
		typename std::result_of<FuncType(Args...)>::type >::type{
		return std::forward<FuncType>(f)(std::forward<Args>(args)...);	
	}

	// member function pointer + object pointer
	template<class FuncType, class ClassType, class... Args>
	inline auto Invoke( FuncType&& func, ClassType&& obj, Args&&... args)
		-> typename std::enable_if<is_member_function_ptr<FuncType>::value 
								&& std::is_pointer<decay_t<ClassType>>::value,
		typename function_traits<FuncType>::return_type>::type {
		return (std::forward<ClassType>(obj)->*std::forward<FuncType>(func))( std::forward<Args>(args)... );	
	}	
	
	// member function pointer + object 
	template<class FuncType, class ClassType, class... Args>
	inline auto Invoke( FuncType&& func, ClassType&& obj, Args&&... args)
		-> typename std::enable_if<is_member_function_ptr<FuncType>::value 
								&& not std::is_pointer<decay_t<ClassType>>::value,
		typename function_traits<FuncType>::return_type>::type {
		return (std::forward<ClassType>(obj).*std::forward<FuncType>(func))( std::forward<Args>(args)... );	
	}	
/*	struct Invoker{
		//functional Type like std::function/operator() type/lambda/function pointer/const function pointer/function reference
		template<class FuncType, class... Args>		
		inline typename std::result_of<FuncType(Args...)>::type operator()(FuncType&& f, Args&&... args){
			return std::forward<FuncType>(f)(std::forward<Args>(args)...);	
		}

		// for member function pointer
		template<class RetType, class ClassType, class... Args>
		inline RetType operator()( RetType(ClassType::*f)(Args...), ClassType* obj_ptr, Args&&... args){
			return (obj_ptr->*f)( std::forward<Args>(args)... );	
		}	

		template<class RetType, class ClassType, class... Args>
		inline RetType operator()( RetType(ClassType::*f)(Args...) , ClassType& obj, Args&&... args){
			return (obj.*f)( std::forward<Args>(args)... );	
		}	

		template<class RetType, class ClassType, class... Args>
		inline RetType operator()( RetType(ClassType::*f)(Args...) , ClassType&& obj, Args&&... args){
			return (obj.*f)( std::forward<Args>(args)... );	
		}	

		// for temperal member function pointer
		template<class RetType, class ClassType, class... Args>
		inline RetType operator()( RetType(ClassType::*f)(Args...) &&, ClassType&& obj, Args&&... args){
			return (std::move(obj).*f)( std::forward<Args>(args)... );	
		}	

		// for reference member function pointer
		template<class RetType, class ClassType, class... Args>
		inline RetType operator()( RetType(ClassType::*f)(Args...) &, ClassType* obj_ptr, Args&&... args){
			return (obj_ptr->*f)( std::forward<Args>(args)... );	
		}	

		template<class RetType, class ClassType, class... Args>
		inline RetType operator()( RetType(ClassType::*f)(Args...) &, ClassType& obj, Args&&... args){
			return (obj.*f)( std::forward<Args>(args)... );	
		}	

		// for const member function pointer
		template<class RetType, class ClassType, class... Args>
		inline RetType operator()( RetType(ClassType::*f)(Args...) const, const ClassType* obj_ptr, Args&&... args){
			return (obj_ptr->*f)( std::forward<Args>(args)... );	
		}	

		template<class RetType, class ClassType, class... Args>
		inline RetType operator()( RetType(ClassType::*f)(Args...) const, ClassType* obj_ptr, Args&&... args){
			return (obj_ptr->*f)( std::forward<Args>(args)... );	
		}	

		template<class RetType, class ClassType, class... Args>
		inline RetType operator()( RetType(ClassType::*f)(Args...) const, const ClassType& obj, Args&&... args){
			return (obj.*f)( std::forward<Args>(args)... );	
		}	

		template<class RetType, class ClassType, class... Args>
		inline RetType operator()( RetType(ClassType::*f)(Args...) const, ClassType& obj, Args&&... args){
			return (obj.*f)( std::forward<Args>(args)... );	
		}	

		template<class RetType, class ClassType, class... Args>
		inline RetType operator()( RetType(ClassType::*f)(Args...) const, const ClassType&& obj, Args&&... args){
			return (obj.*f)( std::forward<Args>(args)... );	
		}	

		template<class RetType, class ClassType, class... Args>
		inline RetType operator()( RetType(ClassType::*f)(Args...) const, ClassType&& obj, Args&&... args){
			return (obj.*f)( std::forward<Args>(args)... );	
		}	

		// for const reference member function pointer
		template<class RetType, class ClassType, class... Args>
		inline RetType operator()( RetType(ClassType::*f)(Args...) const&, const ClassType* obj_ptr, Args&&... args){
			return (obj_ptr->*f)( std::forward<Args>(args)... );	
		}	

		template<class RetType, class ClassType, class... Args>
		inline RetType operator()( RetType(ClassType::*f)(Args...) const&, const ClassType& obj, Args&&... args){
			return (obj.*f)( std::forward<Args>(args)... );	
		}	
	};	
	
	template<class... Args>
	auto Invoke(Args&&... args) -> decltype(Invoker()(std::forward<Args>(args)...)){
		return Invoker()(std::forward<Args>(args)...);	
	}
*/
}
