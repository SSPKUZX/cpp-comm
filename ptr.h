#pragma once 

#include <memory>

namespace utl 
{

	template< class _Tp >
	using sptr = std::shared_ptr< _Tp >;
	template< class _Tp >
	using uptr = std::unique_ptr< _Tp >;

#if __cplusplus >= 201402L ||                                              \
    (defined __cpp_lib_make_unique && __cpp_lib_make_unique >= 201304L) || \
    (defined(_MSC_VER) && _MSC_VER >= 1900)

/* using override */ using std::make_unique;

#else
	// simple class
	template< class _Tp, class... _Args>
	auto make_unique( _Args&&... args)
		-> typename std::enable_if< !std::is_array<_Tp>::value, std::unique_ptr< _Tp>>::type 
	{
		return uptr<_Tp>( new _Tp( std::forward<_Args>(args)... ) );
	}

	// class array
	template< class _Tp>
	auto make_unique( std::size_t size)	 
		-> typename std::enable_if< std::is_array<_Tp>::value  && std::extent<_Tp>::value == 0, std::unique_ptr<_Tp> >::type
	{
		using Elem = typename std::remove_extent<_Tp>::type;
		return uptr<_Tp>( new Elem[size]() );
	}

	// disable T[N]  with a known bound
	template< class _Tp, class... _Args>
	auto make_unique( _Args&&... args)	->typename std::enable_if< std::is_array<_Tp>::value && std::extent<_Tp>::value != 0, void>::type = delete;

#endif

} // end of utl 

