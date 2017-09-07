#pragma once

#include <functional>
#include <iostream>

#include "traits.h"

namespace utl 
{
	template <class F>
	struct Pipeable
	{
		private:
			F f;
		public:
		    Pipeable(F&& f) : f(std::forward<F>(f)) {}
			     
			template<class... Xs>
			auto operator()(Xs&&... xs) -> decltype(std::bind(f, std::placeholders::_1, std::forward<Xs>(xs)...)) const {
				return std::bind(f, std::placeholders::_1, std::forward<Xs>(xs)...);
			}
	};
	    
	template <class F>
	Pipeable<F> pipelize(F&& f) { return Pipeable<F>{std::forward<F>(f)}; }
	    
	template<class T, class F>
	auto operator|(T&& x, const F& f) -> decltype(f(std::forward<T>(x)))
	{
		return f( std::forward<T>(x) );
	}

	using namespace std;
     
	template<class F, class T = typename function_traits<F>::template args<0>::type, typename R= std::vector<T> >
	std::function<R(R)> where(F&& pred){
		return [pred]( R const& src){
			R dest;	
			for(auto& item : src){
				if( pred(item) ){
					dest.push_back(item);	
				}	
			}	
			return std::move(dest);
		};
	}
	
	template <class F, class T = typename function_traits<F>::template args<0>::type, class D = typename std::result_of<F(T)>::type, class R = std::vector<D> >
	std::function<R(std::vector<T> const&)> map(F&& mapper)
	{
		return [mapper]( std::vector<T> const& src){
			R dest;
		    for (auto& item : src){
				dest.push_back(mapper(item));
			}
			return std::move(dest);
		};
	}
	
	struct Logger{
		template<class T>
		void operator()(std::vector<T> const& logs) const{
			for (auto& item : logs){
				std::cout << item << "\t";
			}
			std::cout << std::endl;
		}
	};
	
	Logger log() { return Logger(); }
     
/*
	template <typename T>
	T whereInList(const T& list, std::function<bool(typename T::value_type)> f)
	{
	   	 T result;
	   	 for (auto& item : list){
			if (f(item)){
				result.push_back(item);
			}
		 }
	   	 return move(result);
	}
     
	template <typename T>
	T mapToList(const T& list, std::function< typename T::value_type( typename T::value_type  )> f)
	{
		T result;
	    for (auto& item : list){
			result.push_back(f(item));
		}
		return move(result);
	}
	template <typename T>
	T logList(const T& list)
	{
		std::cout << "List: ";
	    for (auto& item : list){
		   std::cout << item << " ";
		}
		std::cout << std::endl;
		return list;
	}
 */    

	template <typename F>
	auto piped1Arg(F f) -> decltype(pipelize(std::bind(f, placeholders::_1)))
	{
		return pipelize(std::bind(f, placeholders::_1));
	}
     
     
	template <typename F>
	auto piped2Args(F f) -> decltype(pipelize(std::bind(f, placeholders::_1, placeholders::_2)))
	{
		return pipelize(std::bind(f, placeholders::_1, placeholders::_2));
	}

}
