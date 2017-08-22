#include <vector>
#include <list>
#include <functional>
#include <iostream>

template< class _ContainerType >
class wrapped_container
{
	private:
		_ContainerType raw_list;
		using _Tp = typename _ContainerType::value_type;
		std::vector< std::function< bool( _Tp const&)> > filters; 

	public:
		wrapped_container( _ContainerType const& list) : raw_list( list ){}

		template< class _Filter >
		wrapped_container& filter( _Filter&& filter1 )
		{
			filters.push_back( std::function<bool( _Tp const& )>( std::forward<_Filter>(filter1) ) );
			return *this;
		};

		_ContainerType result()
		{
			_ContainerType result_container;
			for(auto const& ele : raw_list )	
			{
				if( isValid( ele ) )
					result_container.push_back( ele );
			}
			return result_container;
		}

	private:
		bool isValid( _Tp const& ele)
		{
			for( auto const& _filter : filters )
				if( !_filter(ele) )
					return false;
			return true;
		}

};

template<class _ContainerType>
wrapped_container< _ContainerType > wrap( _ContainerType const& list)
{
	return	wrapped_container<_ContainerType>( list );
}

int main( int argc, char* argv[])
{
	std::vector<int> list = {1,2,3,4,5};
	for( auto const& ele : wrap(list).filter( []( int const& ele){ return ele > 3; } ).result() )
	{
		std::cout << "\t" << ele;
	}
	std::cout << std::endl;

	std::list<int> list2 = {1,2,3,4,5};
	for( auto const& ele : wrap(list2).filter( []( int const& ele){ return ele > 3; } )
									  .filter( []( int const& ele){ return ele % 2 == 0; }).result()  )
	{
		std::cout << "\t" << ele;
	}
	std::cout << std::endl;

	return 0;
}
