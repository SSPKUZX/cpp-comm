#pragma once

namespace alg 
{
namespace detail
{
	template< class InputIterator, class T>
	class findee
	{
		public:
			findee( bool isFound, InputIterator it) : m_not_end( isFound ), m_it( it){} 

			// return if the iterator is valid( not end) 
			operator bool() const { return m_not_end; }
			// retrieve the raw iterator
			inline InputIterator operator()(){ return m_it; } // support for sets/maps's ambiguous operations

			// behave like a iterator
			inline auto operator*() -> decltype(*std::declval<InputIterator>())
			{ return *m_it; } 
			inline InputIterator operator->(){ return m_it; }
			
		private:
			bool			m_not_end;
			InputIterator	m_it;
	};

	template< class InputIterator>
	inline findee<InputIterator> make_findee( bool found, InputIterator it){
		return findee<InputIterator>( found, it );
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

}// end of alg 
