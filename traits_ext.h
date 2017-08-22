#pragma once

#include <type_traits>
#include <cstdint>
#include <functional>

namespace comm
{

	//Note: return type or parameter type can be convertible: is_functional( int(double), double(int) ) = true
	//		any return type can convert to void : is_functional< int(int), void(int)> = true
	
	// decide if it can call like this Callable(Args...)
	template< typename Callable, typename... Args> 
	struct is_functional 
	{ 
		private:
			template <typename _Callable, typename... _Args>
			static auto check(int) -> decltype( std::bind( std::declval<_Callable>(), std::declval<_Args>()...)(), std::true_type() );
			template <typename _Callable, typename... _Args>
			static auto check(...) -> std::false_type;
	
		public:
			enum { value = decltype(check<Callable, Args...>(0))::value };
			//enum { value = std::is_same< std::true_type, decltype( check<Callable, Args...>(0) )>::value };
	}; 

	// decide if it is a function object
	template< typename Callable> 
	struct is_functional< Callable >
	{ 
	/*	typedef char yes_type[1];
		typedef char no_type[2];
	
		template <typename Q>
		static yes_type& check(decltype(&Q::operator()));
		template <typename Q>
		static no_type& check(...);
	
		public:
		enum { value = sizeof(check<Callable>(nullptr)) == sizeof(yes_type) };
		*/
		private:
			template<typename _Callable> static auto check(int)	-> decltype( &_Callable::operator(), std::true_type() ); 
			template<typename _Callable> static auto check(...)	-> std::false_type;    
		
		public: 
			enum { value = std::is_same< std::true_type, decltype( check<Callable>(0) ) >::value }; 
	}; 

	// decide if it matches a certain function pattern
	template<class Callable, class R, class...Args>
	struct is_functional< Callable, R(Args...)>
	{
		enum { value = std::is_assignable<std::function<R(Args...)>, Callable>::value };
	//	enum { value = std::is_assignable<std::function<R(Args...)>, decltype(std::bind(std::declval<Callable>() ) )>::value };
	};
	
	// ################# has func member ###################
	#define has_member(member) \
	template< typename T, typename... Args> \
	struct has_member_##member \
	{ \
		private:\
			template<typename U> static auto check(int)	-> decltype( std::declval<U>().member( std::declval<Args>()... ), std::true_type() ); \
			template<typename U> static auto check(...)	-> std::false_type;    \
		\
		public: \
			enum { value = std::is_same< std::true_type, decltype( check<T>(0) ) >::value }; \
	}; \


	// ################# obtain xth class in variadic classes  ###################
/*	template< size_t N, class... Args>
	struct get_type;
	
	// disable N >= sizeof...(Args)
	template< size_t N >
	struct get_type< N > 
	{
		//using type = void; 
	};

	template< size_t N, class T, class... Args>
	struct get_type< N, T, Args...> //typename std::enable_if< (N>1),
	{
		using type = typename get_type<N-1, Args...>::type;
	};
	
	template<class T, class... Args>
	struct get_type<0, T, Args...>
	{
		using type = T;	
	};
*/

/*
	template< class... Args>
	struct variadic_trait;

	template< class FirstArg, class... Args>
	struct variadic_trait< FirstArg, Args...>
	{
		using first_arg = FirstArg;
	};
*/

	// ################# how many parameters the function needs( how to count args in lambda remains unknown)  ###################
	
	// by default, it's a function class with operator() overloaded
	template< class Callable>
	struct argument_count
	{
		static const uint32_t value = argument_count< decltype(&Callable::operator())>::value - 1;		
	};
	
	// specialize for raw function pointer
	template< class R, class... Args>
	struct argument_count< R( Args...)>	
	{
		static const uint32_t value = sizeof...(Args);		
	};
	
	// specialize for member function
	template< class R, class Type, class... Args>
	struct argument_count< R(Type::*)( Args...)>	
	{
		// including this pointer or reference of class Type
		static const uint32_t value = sizeof...(Args) + 1;		
	};

	



	// ################# obtain xth parameter type in Callable  ###################

/*	template< class Callable>
	struct func_trait< typename std::enable_if< is_functional<Callable>::value, decltype(&Callable::operator()) >::type > 
	{
		using first_arg = typename func_trait< decltype(&Callable::operator()) >::first_arg;
	};
	
	template< class Callable, class R, class T, class... Args>
	struct func_trait<typename std::enable_if< std::is_assignable< std::function<R(T,Args...)>, decltype( std::bind( std::declval<Callable> ) )>::value, Callable>::type 
	{
		using first_arg = T;
	};
	*/

	// by default, function object with operator() overloaded 
	template< class Callable>
	struct func_trait 
	{
		using first_arg = typename func_trait< decltype(&Callable::operator()) >::first_arg;
	};
	
	// specialize for raw function pointer
	template< class R, class T, class... Args>
	struct func_trait< R( T, Args...)>	
	{
		using first_arg = T;
	};
	
	// specialize for member function
	template< class R, class Type, class T, class... Args>
	struct func_trait< R(Type::*)( T, Args...)>	
	{
		using first_arg = T;
	};
	

} // end of comm
