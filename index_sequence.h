#pragma once

#include <cstddef>

namespace utl{

	template <std::size_t...>
	struct IndexSequence {};
	
	template <std::size_t N, std::size_t... Is>
	struct MakeIndexSequence : MakeIndexSequence<N - 1, N - 1, Is...> {};
	
	template <std::size_t... Is>
	struct MakeIndexSequence<0, Is...> : IndexSequence<Is...> {};

}
