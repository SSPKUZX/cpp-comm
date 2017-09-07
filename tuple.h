#include <tuple>
#include "call.h"

namespace comm
{
	template <std::size_t...>
	struct IndexSequence {};
	
	template <std::size_t N, std::size_t... Is>
	struct MakeIndexSequence : MakeIndexSequence<N - 1, N - 1, Is...> {};
	
	template <std::size_t... Is>
	struct MakeIndexSequence<0, Is...> : IndexSequence<Is...> {};

	template< class... Args>
	using IndexSequenceOf = MakeIndexSequence< sizeof...(Args)>;


	template<class F, class Tuple, size_t... Indexes>
	auto call_on_index( F&& f, Tuple&& t, IndexSequence<Indexes...>)
		->decltype(call_any( std::forward<F>(f), std::get<Indexes>(std::forward<Tuple>(t))... ) )
	{
		return call_any( std::forward<F>(f), std::get<Indexes>(std::forward<Tuple>(t))... ); 
	}
	
	template< class F, class... Args>
	auto call_through_tuple( F&& f, Args&&... args)
		->decltype(call_on_index( std::forward<F>(f),std::make_tuple(args...), IndexSequenceOf<Args...>() ) )
	{
		return call_on_index( std::forward<F>(f),std::make_tuple(args...), IndexSequenceOf<Args...>() );
	}

} // end of comm
