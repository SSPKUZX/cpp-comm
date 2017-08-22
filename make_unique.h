#pragma once 

#include <memory>

namespace cmn
{
	template< class Tp >
		using up = std::unique_ptr< Tp >;

#if __cplusplus >= 201402L ||                                              \
	(defined __cpp_lib_make_unique && __cpp_lib_make_unique >= 201304L) || \
	(defined(_MSC_VER) && _MSC_VER >= 1900)

	using std::make_unique;

#else
	// simple class
	template< class Tp, class... Args>
		auto make_unique( Args&&... args)
		-> typename std::enable_if< !std::is_array<Tp>::value, up<Tp>>::type 
		{
			return up<Tp>( new Tp( std::forward<Args>(args)... ) );
		}

	// class array
	template< class Tp>
		auto make_unique( std::size_t size)	 
		-> typename std::enable_if< std::is_array<Tp>::value  && std::extent<Tp>::value == 0, up<Tp> >::type
		{
			using Elem = typename std::remove_extent<Tp>::type;
			return up<Tp>( new Elem[size]() );
		}

	// disable T[N]  with a known bound
	template< class Tp, class... Args>
		auto make_unique( Args&&... args)	->typename std::enable_if< std::is_array<Tp>::value && std::extent<Tp>::value != 0, void>::type = delete;

#endif

} // end of comm

