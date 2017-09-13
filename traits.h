//https://stackoverflow.com/questions/7943525/is-it-possible-to-figure-out-the-parameter-type-and-return-type-of-a-lambda
//https://github.com/kennytm/utils/blob/master/traits.hpp

#pragma once

#include <tuple>
#include <functional>

namespace utl 
{
	template<bool Enabled, class Type=void>
	using enable_if_t = typename std::enable_if<Enabled, Type>::type;
	template<class Signature>
	using result_of_t = typename std::result_of<Signature>::type;
	template<bool Cond, class T, class U>
	using conditional_t	= typename std::conditional<Cond, T, U>::type;
	template<std::size_t I, class Tuple>
	using tuple_element_t = typename std::tuple_element<I, Tuple>::type;	
	template<class T>
	using decay_t = typename std::decay<T>::type;

	template<class Signature>
	struct is_invokable : public std::false_type{};

	template<typename F, typename... Args>        
	struct is_invokable<F(Args...)>
	{
		private:	
			template< class F2, class R2=result_of_t<F2(Args...)> >
			static constexpr std::true_type check(int);
			template<class F2>
			static constexpr std::false_type check(...);
		public:
			enum { value = decltype(check<F>(0))::value };
	};              

	// if T is signature of R(Args...) 
	template<class T, class Signature>
	struct is_signature : public std::false_type{};

	template<class F, class R, class...Args>
	struct is_signature<F, R(Args...)>{
		private:	
			template< class F2, class R2=result_of_t<F2(Args...)> >
			static constexpr auto check(int)
			-> conditional_t<std::is_same<R,R2>::value, std::true_type, std::false_type>;
			template<class F2>
			static constexpr std::false_type check(...);
		public:
			enum { value = decltype(check<F>(0))::value };
	};

	template<size_t I, class... Args>
	struct nth_type{
		using type = tuple_element_t<I, std::tuple<Args...>>;	
	};

	template<size_t I, class... Args>
	using nth_type_t = typename nth_type<I,Args...>::type;
	
	template<class T,class... Args>
	struct index_of{};

	template<class T>
	struct index_of<T>
	{
		static constexpr int8_t	value = -1; 	
	};

	template<class T, class U, class... Args>
	struct index_of<T, U, Args...>
	{
		static constexpr int8_t value = std::is_same<T,U>::value ?
			0 : (-1 != index_of<T,Args...>::value) ? 
			(1 + index_of<T,Args...>::value) : -1;
	};

	template<class T, class... Args>
	struct one_of
	{
		static constexpr bool value = -1!=index_of<T,Args...>::value;
	};

	template<class... Args>
	struct has_duplicate{};

	template<>
	struct has_duplicate<>
	{
		static constexpr bool value = false; 
	};

	template<class T, class... Args>
	struct has_duplicate<T, Args...> 
	{
		static constexpr bool value = one_of<T,Args...>::value || has_duplicate<Args...>::value;
	};


	// false by default
	template<class Signature, class Enable=void>
	struct function_traits : public std::false_type{};

	// for function
	template<class RetType, class... Args>
	struct function_traits<RetType(Args...) > : public std::true_type{
		static const size_t arity = sizeof...(Args);
		using return_type = RetType;	
		using args_tuple_type = std::tuple<decay_t<Args>...>;
		template<size_t I>
		struct args{
			using type = nth_type_t<I, Args...>;	
		};	
	};

	//for reference function
	template<class RetType, class... Args>
	struct function_traits<RetType(&)(Args...)> : public function_traits<RetType(Args...)>{};

	//for const function
	template<class RetType, class... Args>
	struct function_traits<RetType(Args...) const> : public function_traits<RetType(Args...)>{};

	// for function pointer
	template<class RetType, class... Args>
	struct function_traits<RetType(*)(Args...) > : public function_traits<RetType(Args...)>{};

	// for member function pointer, ClassType* is the first parameter type
	template<class RetType, class ClassType, class... Args>
	struct function_traits<RetType(ClassType::*)(Args...) >{
		static const size_t arity = 1+sizeof...(Args);
		using return_type = RetType;	
		using args_tuple_type = std::tuple<ClassType*,decay_t<Args>...>;
		using args_tuple_type_ = std::tuple<decay_t<Args>...>; // helper for function class 
		template<size_t I>
		struct args{
			static_assert(I>0, "I in args should be greater than 0 when Signature is member function");
			using type = nth_type_t<I-1, Args...>;	
		};	
	};

	// for const member function pointer
	template<class RetType, class ClassType, class... Args>
	struct function_traits<RetType(ClassType::*)(Args...) const> : public function_traits<RetType(ClassType::*)(Args...)>{};

	// is operator() defined
	template<class T>
	struct is_function{
		private:
			template<typename Callable> static auto check(int)	-> decltype( &Callable::operator(), std::true_type() ); 
			template<typename Callable> static auto check(...)	-> std::false_type;    
		public:
			enum { value = decltype(check<T>(0))::value };
	};
	// for lambda && std::function && class with customized operator() 
	// ClassType* is not necessary for above types compared to member function 
	template<class Signature >
	struct function_traits<Signature, enable_if_t<is_function<Signature>::value>> {
		using ft= function_traits<decltype(&Signature::operator())>;
		static const size_t arity = ft::arity-1;
		using return_type = typename ft::return_type;	
		using args_tuple_type = typename ft::args_tuple_type_;
		template<size_t I>
		struct args{
			using type = typename ft::template args<I+1>::type;	
		};	
	};

	template<size_t I, class... Args>
	struct placeholder_index;
	
	template<size_t I>
	struct placeholder_index<I>{
		enum{ value = -1,};
	};
	
	template<size_t I, class T, class... Args>
	struct placeholder_index<I, T, Args...>{
		static_assert(I>0, "I should be greater than 0 in placeholder_index, because \
				std::is_placeholder<_1>::value is 1 while it's zero_index parameter");
		enum{ pre_value	= placeholder_index<I, Args...>::value, 
			  value = (I == std::is_placeholder<decay_t<T>>::value) ? 
				  0 : ((pre_value==-1)?-1:(1+pre_value)),
		};
	};
	
	
	template<class... Args>
	struct placeholder_count;
	
	template<>
	struct placeholder_count<>{
		enum{ value = 0,};
	};
	
	template<class T, class... Args>
	struct placeholder_count<T, Args...>{
		enum{ pre_value	= placeholder_count<Args...>::value, 
			  value = (std::is_placeholder<typename std::decay<T>::type>::value ? 
				  1 : 0) + pre_value,
		};
	};
	
	// support for bind expression
	template<class Signature, class... Args >
	struct function_traits<std::__1::__bind<Signature, Args...> > : public std::true_type{
		static const size_t arity = placeholder_count<Args...>::value;
		using return_type	= typename function_traits<Signature>::return_type;
		// no args_tuple_type set
		template<size_t I>
		struct args{
			static_assert(placeholder_index<I+1, Args...>::value>-1, "specified placeholder is not found, please check");
			// if I is 0, then the index of std::placeholders::_1 should be put in 
			// placeholder_index
			using type = typename function_traits<Signature>::template args<placeholder_index<I+1, Args...>::value>::type;	
		};
	};

	// ################# has func #member ###################
	#define has_member(member) \
	template< typename T, typename... Args> \
	struct has_member_##member \
	{ \
		private:\
			template<typename U> static auto check(int)	-> decltype( std::declval<U>().member( std::declval<Args>()... ), std::true_type() ); \
			template<typename U> static auto check(...)	-> std::false_type;    \
		\
		public: \
			enum { value = decltype(check<T>(0))::value }; \
	}; \


} // end of utl 
