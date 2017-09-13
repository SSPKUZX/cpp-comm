#pragma once

namespace utl 
{
namespace detail
{
	template< class iterator>
	class findee
	{
		public:
			findee( bool isFound, iterator const& it) : m_is_found( isFound ), m_it( it){} 
			operator bool() const { return m_is_found; }
			iterator const& get() const { return m_it; }

		private:
			bool		m_is_found;
			iterator	m_it;
	};

	template< class iterator>
	inline findee<iterator> make_findee( bool found, iterator it){
		return findee<iterator>( found, it );
	}

	// for unordered_map/map/unordered_set/set
	template< class Container, class Key>
	class finder 
	{
		public:
			inline auto operator()( Container const& container, Key const& key)
					->decltype( make_findee( false, container.find(key) ) )
			{
				auto it = container.find(key);
				return  make_findee( (it != container.end()), it); 
			}
	};
	
	// specialize for deque/vector/list
	template< template<class, class> class SeqContainer, class Elem>
	class finder< SeqContainer< Elem, std::allocator<Elem> >, Elem>
	{
		public:		
			inline auto operator()( SeqContainer< Elem, std::allocator<Elem> > const& container, Elem const& elem)
					->decltype( make_findee( false, std::find( container.begin(), container.end(), elem ) ) )
			{
				auto it = std::find( container.begin(), container.end(), elem );
				return make_findee( (it != container.end()), it );
			}
	};

	// specialize for deque/vector/list of find_if
	template< template<class, class> class SeqContainer, class Elem, class Predicate>
	class finder< SeqContainer< Elem, std::allocator<Elem> >, Predicate>
	{
		public:		
			inline auto operator()( SeqContainer< Elem, std::allocator<Elem> > const& container, Predicate const& predicate)
					->decltype( make_findee( false, std::find_if( container.begin(), container.end(), predicate ) ) )
			{
				auto it = std::find_if( container.begin(), container.end(), predicate );
				return make_findee( (it != container.end()), it );
			}
	};
} // end of detail

template<class Container, class Equaler>
auto find( Container const& container, Equaler const& equaler)
		->detail::findee< typename Container::const_iterator >
{
	static  detail::finder< Container, Equaler> finder;
	return  finder( container, equaler ); 
}

}// end of utl 
