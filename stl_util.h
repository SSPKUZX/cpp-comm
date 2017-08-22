#pragma once

namespace comm
{
	namespace detail
	{
		template<class iterator>
		class find_result;
	}
	template<class _Container, class _Equaler>
	auto find( _Container const& container, _Equaler const& equaler)
		->detail::find_result< typename _Container::const_iterator >;

}// end of comm

#include "stl_util_inl.h"
