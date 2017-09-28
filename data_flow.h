#pragma once

#include <fstream>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

#include "traits.h"

namespace utl{

	template<class T, class F>
	inline auto operator|(T&& x, F&& f) -> decltype(f(std::forward<T>(x)))
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
		Logger( std::ostream& out = std::cout) : m_out(out){}

		template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T>
		SeqContainer<T> const& operator()(SeqContainer<T> const& logs) const{
			copy(logs.begin(), logs.end(), ostream_iterator<T>(m_out, "\n"));
			return logs;
		}

		private:
			std::ostream&	m_out;
	};
	
	// to use in map or filter
	using std::placeholders::_1;

	Logger log() { return Logger(); }
	// read records from file
	vector<string> read_file(std::string const& filePath){ 
		fstream input(filePath);
		vector<string> ips;
		string line;
		while( input >> line ){
			ips.push_back(line);
		}
		input.close();
		return std::move(ips);
	}
	
	// sort elements
	struct sorter{
		template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T>
		inline SeqContainer<T>& operator()( SeqContainer<T>& records) const{
			std::sort(records.begin(), records.end());	
			return records;
		}
	
		template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T>
		inline SeqContainer<T> operator()( SeqContainer<T> records) const{ // support for movable&const_refer records 
			std::sort(records.begin(), records.end());	
			return std::move(records);
		}
	
	};
	
	inline sorter sort(){ return sorter(); }

	// unique 
	struct uniquer{
		template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T>
		inline SeqContainer<T>& operator()( SeqContainer<T>& records) const{
			std::sort(records.begin(), records.end());	
			records.remove(std::unique(records.begin(), records.end()), records.end());	
			return records;
		}
	
		template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T>
		inline SeqContainer<T> operator()( SeqContainer<T> records) const{ // support for movable&const_refer records 
			std::sort(records.begin(), records.end());	
			records.remove(std::unique(records.begin(), records.end()), records.end());	
			return std::move(records);
		}
	
	};
	
	inline uniquer unique(){ return uniquer(); }

	// shuffle elements
	struct shuffler{
		template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T>
		inline SeqContainer<T>& operator()( SeqContainer<T>& records) const{
			std::random_shuffle(records.begin(), records.end());	
			return records;
		}
	
		template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T>
		inline SeqContainer<T> operator()( SeqContainer<T> records) const{ // support for movable&const_refer records 
			std::random_shuffle(records.begin(), records.end());	
			return std::move(records);
		}
	
	};
	
	inline shuffler shuffle(){ return shuffler(); }
	
	// calcalute difference set
	template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T> 
	struct difference{
		difference( SeqContainer<T> && records) : m_right_records( std::move(records) ){
			sort(m_right_records.begin(), m_right_records.end());
		}
	
		template< template<class T2, class alloc = std::allocator<T2>> class SeqContainer2, class T2> 
		inline SeqContainer<T> operator()(SeqContainer2<T2>& leftRecords) const{
			SeqContainer<T> diff;
			if( is_sorted( leftRecords.begin(), leftRecords.end() ) ){
				sort(leftRecords.begin(), leftRecords.end());
			}
			set_difference(m_right_records.begin(), m_right_records.end(), leftRecords.begin(), leftRecords.end(), back_inserter(diff));	
			return std::move(diff);
		}	

		template< template<class T2, class alloc = std::allocator<T2>> class SeqContainer2, class T2> 
		inline SeqContainer<T> operator()(SeqContainer2<T2>&& leftRecords) const{ // support for value&const_reference type
			return operator()(leftRecords); 
		}	
	
		private:
			SeqContainer<T>	m_right_records;
	};
	
	
	template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T> 
	inline difference<SeqContainer, T> differenciate(SeqContainer<T> records){
		return difference<SeqContainer,T>(std::move(records));
	}
	
	// calculate intersection set
	template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T> 
	struct intersection{
		intersection( SeqContainer<T> && records) : m_right_records( std::move(records) ){
			sort(m_right_records.begin(), m_right_records.end());
		}
	
		template< template<class T2, class alloc = std::allocator<T2>> class SeqContainer2, class T2> 
		inline SeqContainer<T> operator()(SeqContainer2<T2>& leftRecords) const{
			SeqContainer<T> intersect;
			if( !is_sorted( leftRecords.begin(), leftRecords.end() ) ){
				sort(leftRecords.begin(), leftRecords.end());
			}
			set_intersect(m_right_records.begin(), m_right_records.end(), leftRecords.begin(), leftRecords.end(), back_inserter(intersect));	
			return std::move(intersect);
		}	

		template< template<class T2, class alloc = std::allocator<T2>> class SeqContainer2, class T2> 
		inline SeqContainer<T> operator()(SeqContainer2<T2>&& leftRecords) const{ // support for value&const_reference type
			return operator()(leftRecords); 
		}	
	
		private:
			SeqContainer<T>	m_right_records;
	};
	
	template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T> 
	inline intersection<SeqContainer, T> intersect(SeqContainer<T> records){
		return intersection<SeqContainer,T>(std::move(records));
	}

	// calculate union set
	template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T> 
	struct unionizer{
		unionizer( SeqContainer<T> && records) : m_right_records( std::move(records) ){
			sort(m_right_records.begin(), m_right_records.end());
		}
	
		template< template<class T2, class alloc = std::allocator<T2>> class SeqContainer2, class T2> 
		inline SeqContainer<T> operator()(SeqContainer2<T2>& leftRecords) const{
			SeqContainer<T> unioned;
			if( !is_sorted( leftRecords.begin(), leftRecords.end() ) ){
				sort(leftRecords.begin(), leftRecords.end());
			}
			set_union(m_right_records.begin(), m_right_records.end(), leftRecords.begin(), leftRecords.end(), back_inserter(unioned));	
			return std::move(unioned);
		}	

		template< template<class T2, class alloc = std::allocator<T2>> class SeqContainer2, class T2> 
		inline SeqContainer<T> operator()(SeqContainer2<T2>&& leftRecords) const{ // support for value&const_reference type
			return operator()(leftRecords); 
		}	
	
		private:
			SeqContainer<T>	m_right_records;
	};
	
	template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T> 
	inline unionizer<SeqContainer, T> intersect(SeqContainer<T> records){
		return unionizer<SeqContainer,T>(std::move(records));
	}

} // end of utl
