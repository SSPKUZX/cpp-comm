#pragma once

#include "traits.h"
#include "apply_tuple.h"
#include "ptr.h"

namespace utl{
	using namespace utl::detail::apply_tuple;
	
	// make indexes for placeholders 
	// (_1,int,_2,char) -> <0,2>
	template<std::size_t N, class... Args>
	struct make_phindexes;
	
	template<std::size_t N, class... Args, std::size_t... Is>
	struct make_phindexes< N,std::tuple<Args...>,IndexSequence<Is...> > 
		: make_phindexes< N-1,std::tuple<Args...>, typename std::conditional<
		0!=std::is_placeholder<typename std::decay<typename nth_type<N-1,Args...>::type>::type>::value,
		IndexSequence<N-1,Is...>, IndexSequence<Is...> >::type >{};
	
	template<class... Args, std::size_t... Is>
	struct make_phindexes<0, std::tuple<Args...>, IndexSequence<Is...> > : IndexSequence<Is...>{};
	
	// make indexes for non-placeholders 
	// (_1,int,_2,char) -> <1,3>
	template<std::size_t N, class... Args>
	struct make_nonphindexes;
	
	template<std::size_t N, class... Args, std::size_t... Is>
	struct make_nonphindexes< N,std::tuple<Args...>,IndexSequence<Is...> > 
		: make_nonphindexes< N-1,std::tuple<Args...>, typename std::conditional<
		0==std::is_placeholder<typename std::decay<typename nth_type<N-1,Args...>::type>::type>::value,
		IndexSequence<N-1,Is...>, IndexSequence<Is...> >::type >{};
	
	template<class... Args, std::size_t... Is>
	struct make_nonphindexes<0, std::tuple<Args...>, IndexSequence<Is...> > : IndexSequence<Is...>{};
	
	// make value list of std::is_placeholder 
	// (_2,int,_1,char) -> <1,0>
	template<class Tuple, std::size_t... Is>
	inline auto phvs( IndexSequence<Is...>)
		-> IndexSequence< (std::is_placeholder<typename std::decay<typename std::tuple_element<Is,Tuple>::type>::type>::value-1)...>	
	{
		return IndexSequence< (std::is_placeholder<typename std::decay<typename std::tuple_element<Is,Tuple>::type>::type>::value-1)...>{};	
	}
	template<class... Args>
	struct PHVsGenerator{
		inline constexpr auto operator()() const
		-> decltype(phvs<std::tuple<Args...>>( make_phindexes< sizeof...(Args),std::tuple<Args...>,IndexSequence<> >{}) ){
			return  phvs<std::tuple<Args...>>( make_phindexes< sizeof...(Args),std::tuple<Args...>,IndexSequence<> >{});	
		}
	};

	// set tpl from rtpl
	template<class... Args, std::size_t... Is, class... RArgs, std::size_t... RIs>
	inline void setTupleValue(std::tuple<Args...>& tpl, IndexSequence<Is...>, std::tuple<RArgs...> const& rtpl, IndexSequence<RIs...>){
		std::tie( std::get<Is>(tpl)... ) = std::make_tuple( std::get<RIs>(rtpl)... );
	}
	
	// the result class returned by Bind
	template<class PHVsGeneratorType, class F, std::size_t... PHIs>
	class Binder{
		public:	
			Binder(F&& functor, typename function_traits<F>::args_tuple_type*&& args_ptr) 
				: m_functor(functor), m_args_uptr(args_ptr){}
	
			// this is the final signature we want 
			auto operator()( typename function_traits<F>::template args<PHIs>::type... args)
				-> typename function_traits<F>::return_type
			{
				setTupleValue(*m_args_uptr, 
							  IndexSequence<PHIs...>{},
							  std::make_tuple( std::forward<typename function_traits<F>::template args<PHIs>::type>(args)... ),
						//	  MakeIndexSequence<sizeof...(PHIs)>{}
							  m_phvs_generator()
						 );	
				return ApplyTuple(m_functor, *m_args_uptr);
			}
	
		private:
			typename std::decay<F>::type						m_functor;	
			uptr<typename function_traits<F>::args_tuple_type>	m_args_uptr;
			PHVsGeneratorType									m_phvs_generator;
	};

	template<class PHVsGenerator, class F, std::size_t... PHIs>
	inline auto constructBinder( F&& f, typename function_traits<F>::args_tuple_type*&& args_values_ptr, IndexSequence<PHIs...> )
		-> Binder<PHVsGenerator,F,PHIs...> {
			return Binder<PHVsGenerator,F,PHIs...>(std::forward<F>(f), std::move(args_values_ptr)); 
	}
	
	template<class F, class... Args>
	auto Bind( F&& f, Args&&... args)
		-> decltype(constructBinder<PHVsGenerator<Args...>>( std::forward<F>(f),
					new typename function_traits<F>::args_tuple_type(),
					make_phindexes< sizeof...(Args),std::tuple<Args...>,IndexSequence<> >{} )
				)
	{
		auto args_values_ptr = new typename function_traits<F>::args_tuple_type(); 
		// set those binded args
		setTupleValue(*args_values_ptr, 
					make_nonphindexes< sizeof...(Args),std::tuple<Args...>,IndexSequence<> >{},
					std::make_tuple( std::forward<Args>(args)... ),
					make_nonphindexes< sizeof...(Args),std::tuple<Args...>,IndexSequence<> >{}
			 );	
		return constructBinder<PHVsGenerator<Args...>>( std::forward<F>(f), std::move(args_values_ptr), 
								make_phindexes< sizeof...(Args),std::tuple<Args...>,IndexSequence<> >{}
				);
	};

	template<class F, std::size_t... Is, class... Args, class R = typename function_traits<F>::return_type,
			 class Binder = std::function<R(typename function_traits<F>::template args<Is>::type...)> >
	inline Binder constructBinder2( F&& f, IndexSequence<Is...>, Args&&... args ){
	 return Binder( std::bind(std::forward<F>(f), std::forward<Args>(args)...) );
	}

	template<class F, class... Args>
	auto Bind2( F&& f, Args&&... args)
		->decltype( constructBinder2( std::forward<F>(f),
								 make_phindexes< sizeof...(Args),std::tuple<Args...>,IndexSequence<> >{},
								 std::forward<Args>(args)... ) ) {
		return constructBinder2( std::forward<F>(f),
							make_phindexes< sizeof...(Args),std::tuple<Args...>,IndexSequence<> >{},
							std::forward<Args>(args)... );	
	}
	
	// for easy use 
	using namespace std::placeholders;
} // end of utl
