#pragma once

#include <ostream>

#include "variadic.h"
#include "index_sequence.h"

namespace utl{
	template<size_t I>
	struct TupleHook{
		template<class Tup>
		void operator()(std::ostream& os, Tup& tup){
			os << '[' << std::get<I>(tup) << ']' << (I+1==std::tuple_size<Tup>::value?' ':',');
		}
	};
	
	template<class Tup, std::size_t... I>
	void chain(utl::IndexSequence<I...> is, std::ostream& os, Tup& tup){
		utl::vindexes<TupleHook, I...>()(os, tup);	
	}
	
	template<class... Args>
	std::ostream& operator<< (std::ostream& os, std::tuple<Args...>& tup){
		chain(utl::MakeIndexSequence<sizeof...(Args)>(), os, tup);
		return os;
	}

}
