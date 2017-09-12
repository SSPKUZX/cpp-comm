/*
 * 
 * pipelize the data flow like this
 *  numbers | map([](x){ return f(x);}) | log();
 * numbers could be one of SeqContainers -> deque&list&vector
 * */
#pragma once

#include <functional>
#include <algorithm>
#include <iostream>

#include "traits.h"

namespace utl 
{
	    
	template<class T, class F>
	auto operator|(T&& x, const F& f) -> decltype(f(std::forward<T>(x)))
	{
		return f( std::forward<T>(x) );
	}

	using namespace std;
     
	template<class T>
	struct filter_impl{
		template<class F>
		filter_impl(F&& pred) : m_functor(std::forward<F>(pred)){}

		// const src mean new container
		// filter should not support SeqContainer type change
	/*	template< template<class TR, class alloc = std::allocator<TR>> class RSeqContainer,
				  template<class T2, class alloc = std::allocator<T2>> class SeqContainer, 
				  typename DT= typename std::decay<T>::type >
		RSeqContainer<DT> operator()( SeqContainer<DT> const& src) const{ 
			RSeqContainer<DT> dest;
			std::copy_if( src.begin(), src.end(), std::back_inserter(dest), m_functor);
			return std::move(dest);
		}*/
		template< template<class T2, class alloc = std::allocator<T2>> class SeqContainer, 
				  typename DT= typename std::decay<T>::type >
		inline SeqContainer<DT> operator()( SeqContainer<DT> const& src) const{ 
			SeqContainer<DT> dest;
			std::copy_if( src.begin(), src.end(), std::back_inserter(dest), m_functor);
			return std::move(dest);
		}

		// filter in place
		template< template<class T2, class alloc = std::allocator<T2>> class SeqContainer, typename DT= typename std::decay<T>::type >
		SeqContainer<DT>& operator()( SeqContainer<DT>& src) const{ 
			src.erase( std::remove_if( src.begin(),src.end(),std::not1(m_functor) ), src.end() );
			return src;
		}

		private:
			std::function<bool(T)>	m_functor;
	};

	template<class F, class T = typename function_traits<F>::template args<0>::type>
	inline filter_impl<T> filter(F&& pred){ return filter_impl<T>(std::forward<F>(pred)); }

	template<class F, class Arg0, class... Args, 
		     class T = typename function_traits<F>::template args<placeholder_index<1,Arg0, Args...>::value>::type >
	inline filter_impl<T> filter(F&& pred, Arg0&& arg0, Args&&... args){ 
		return filter_impl<T>(std::bind(std::forward<F>(pred), std::forward<Arg0>(arg0), std::forward<Args>(args)... )); 
	}

	template<class T, class R>
	struct mapper{
		template<class F>
		mapper( F&& pred) : m_functor( std::forward<F>(pred) ){}

		template< template<class T2, class alloc = std::allocator<T2>> class SeqContainer, 
				  typename DT= typename std::decay<T>::type >
		inline SeqContainer<R> operator()( SeqContainer<DT> const& src) const{
			SeqContainer<R>	dest(src.size());
			std::transform( src.begin(),src.end(),dest.begin(), m_functor);
			return std::move(dest);
		}
	
		private:
			std::function<R(T)>	m_functor;	
	};	

	template <class F, class T = typename function_traits<F>::template args<0>::type, 
			  class R = typename std::result_of<F(T)>::type>
	inline mapper<T,R> map(F&& transformer){ return mapper<T,R>(std::forward<F>(transformer)); }
	// support binding
	template<class F, class Arg0, class... Args, class R = typename function_traits<F>::return_type,
			 class T = typename function_traits<F>::template args<placeholder_index<1,Arg0, Args...>::value>::type>
	inline mapper<T,R> map(F&& pred, Arg0&& arg0, Args&&... args){ 
		return mapper<T,R>( std::bind(std::forward<F>(pred), std::forward<Arg0>(arg0), std::forward<Args>(args)...) ); 
	}

	// support for specifying Return SeqContainer Type
	template<class T, class R, template<class T2, class Alloc=std::allocator<T2>> class RSeqContainer >
	struct mapper_r{
		template<class F>
		mapper_r( F&& pred) : m_functor( std::forward<F>(pred) ){}

		template< template<class T2, class alloc = std::allocator<T2>> class SeqContainer, 
				  typename DT= typename std::decay<T>::type >
		RSeqContainer<R> operator()( SeqContainer<DT> const& src) const{
			RSeqContainer<R>	dest(src.size());
			std::transform( src.begin(),src.end(),dest.begin(), m_functor);
			return std::move(dest);
		}

		private:
			std::function<R(T)>	m_functor;	
	};	

	template < template<class T2, class Alloc=std::allocator<T2>> class RSeqContainer,
			   class F, class T = typename function_traits<F>::template args<0>::type, 
			   class R = typename std::result_of<F(T)>::type>
	inline mapper_r<T,R,RSeqContainer> map(F&& transformer){ return mapper_r<T,R,RSeqContainer>(std::forward<F>(transformer)); }
	// support binding
	template< template<class T2, class Alloc=std::allocator<T2>> class RSeqContainer, 
		      class F, class Arg0, class... Args, class R = typename function_traits<F>::return_type,
			  class T = typename function_traits<F>::template args<placeholder_index<1,Arg0, Args...>::value>::type>
	inline mapper_r<T,R,RSeqContainer> map(F&& pred, Arg0&& arg0, Args&&... args){ 
		return mapper_r<T,R,RSeqContainer>( std::bind(std::forward<F>(pred), std::forward<Arg0>(arg0), std::forward<Args>(args)...) ); 
	}
	
	struct Logger{
		template< template<class T2, class alloc = std::allocator<T2>> class SeqContainer, class T>
		SeqContainer<T> const& operator()(SeqContainer<T> const& logs) const{
			std::for_each(logs.begin(), logs.end(), [](T const& item){
						std::cout << item << "\t";
					});
			std::cout << std::endl;
			return logs;
		}
	};
	
	// to use in map or filter
	using std::placeholders::_1;

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

/*	template <typename F>
	auto piped1Arg(F f) -> decltype(pipelize(std::bind(f, placeholders::_1)))
	{
		return pipelize(std::bind(f, placeholders::_1));
	}
     
     
	template <typename F>
	auto piped2Args(F f) -> decltype(pipelize(std::bind(f, placeholders::_1, placeholders::_2)))
	{
		return pipelize(std::bind(f, placeholders::_1, placeholders::_2));
	}
*/
}
