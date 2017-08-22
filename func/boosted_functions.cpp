#ifndef COMMON_TICKED_FUNCTION_H_
#define COMMON_TICKED_FUNCTION_H_

#include <chrono>
#include <functional>
#include <memory>
#include <iostream>
#include <tuple>
#include <map>
#include <unordered_map>

namespace comm
{

template <typename R, typename... Args>  
std::function<R(Args...)> result_cached_function(R(*func) (Args...))  
{  
	auto result_map = std::make_shared<std::map<std::tuple<Args...>, R>>();  
		  
	return ( [=]( Args... args )
			{  
				std::tuple<Args...> t(args...);  
				if (result_map->find(t) == result_map->end())  
					(*result_map)[t] = func(args...);  
						  
				return (*result_map)[t];  
			});  
}


template< typename R, typename...  Args >  
std::function< R(Args...) > cached_function( R(*func)(Args...), bool needClear = false )  
{  
	using function_type = std::function< R(Args...) >;  
	static std::unordered_map< decltype(func), function_type > functor_map;  
				    
	if ( needClear || functor_map.find(func) == functor_map.end() )  
		functor_map[func] = result_cached_function( func );  
							  
	return functor_map[func];  
}

template<typename R, typename... Args>
std::function< R(Args...)> ticked_function( R(*func)(Args...), double& consumed_seconds)
{
	return ( [&consumed_seconds, func] ( Args... args)
			{
				using std::chrono::steady_clock;
				using std::chrono::duration;

				//calculate seconds consumed in executing this function
				steady_clock::time_point start = steady_clock::now();
				auto result = func( args... );
				consumed_seconds = duration<double>( steady_clock::now() - start ).count();
				
				return result;
			} );
}

} // end of namespace comm

#endif // end of COMMON_TICKED_FUNCTION_H_

//###################### tests as follows ############################//

size_t fibo( size_t n)
{
	return n < 2 ? n : fibo( n-2 ) +  fibo( n-1 );
}

size_t fibo_fast( size_t n)
{
	using namespace comm;

	return n < 2 ? n : cached_function( fibo_fast )( n-2 ) + cached_function( fibo_fast )( n-1 );
}


int main()
{
	using namespace comm;

	size_t n = 10;
	double seconds = 0;
//	std::cout << "fibo(" << n << ") :" << ticked_function( fibo, seconds)( n ) << "\nelapsed time = " << seconds << std::endl;
	std::cout << "fibo_fast(" << n << ") :" << ticked_function( fibo_fast, seconds)( n ) << "\nelapsed time = " << seconds  << std::endl;
}


