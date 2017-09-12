#pragma once

#include<algorithm>

namespace utl 
{
namespace detail
{
	template< class iterator>
	class find_result
	{
		public:
			find_result( bool isFound, iterator const& it) : m_bIsFound( isFound ), m_it( it){} 
			operator bool() const { return m_bIsFound; }
			iterator const& get() const { return m_it; }

		private:
			bool m_bIsFound;
			iterator m_it;
	};

	template< class iterator>
	find_result<iterator> make_result( bool found, iterator it)
	{
		return find_result<iterator>( found, it );
	}

	// for unordered_map/map/unordered_set/set
	template< class _Container, class _Key>
	class finder 
	{
		public:
			//find_result<typename _Container::const_iterator> 
			auto operator()( _Container const& container, _Key const& key)
					->decltype( make_result( false, container.find(key) ) )
			{
				auto it = container.find(key);
				return  make_result( (it != container.end()), it); 
			}
	};
	
	// specialize for deque/vector/list
	template< template<class, class> class _SeqContainer, class _Elem>
	class finder< _SeqContainer< _Elem, std::allocator<_Elem> >, _Elem>
	{
		public:		
			auto operator()( _SeqContainer< _Elem, std::allocator<_Elem> > const& container, _Elem const& elem)
					->decltype( make_result( false, std::find( container.begin(), container.end(), elem ) ) )
			{
				auto it = std::find( container.begin(), container.end(), elem );
				return make_result( (it != container.end()), it );
			}
	};

	// specialize for deque/vector/list of find_if
	template< template<class, class> class _SeqContainer, class _Elem, class _Predicate>
	class finder< _SeqContainer< _Elem, std::allocator<_Elem> >, _Predicate>
	{
		public:		
			auto operator()( _SeqContainer< _Elem, std::allocator<_Elem> > const& container, _Predicate const& predicate)
					->decltype( make_result( false, std::find_if( container.begin(), container.end(), predicate ) ) )
			{
				auto it = std::find_if( container.begin(), container.end(), predicate );
				return make_result( (it != container.end()), it );
			}
	};
} // end of detail

template<class _Container, class _Equaler>
auto find( _Container const& container, _Equaler const& equaler)
		->detail::find_result< typename _Container::const_iterator >
{
	static  detail::finder< _Container, _Equaler> _finder;
	return  _finder( container, equaler ); 
}

}// end of comm 
