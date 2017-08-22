#include <iostream>

namespace cmn
{
	template<uint8_t N, class Tp, class... Types>
	struct nth_type
	{
		static_assert( N>0, "N must be positive in nth_type");
		using type = typename nth_type<N-1, Types...>::type;		
	};

	template<class Tp, class... Types>
	struct nth_type<1, Tp, Types...>
	{
		using type = Tp;		
	};
}
/*
template<std::size_t N, class... Args>
struct nth_type
{
	using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
};
*/
constexpr int k() { return 3;}

class Number 
{
	public:
		constexpr int K() const{ return 3; }
};

int main()
{
	using namespace cmn;

	using first_type	= typename nth_type<1, int, long, char>::type;
	using second_type	= typename nth_type<2, int, long, char>::type;
	using third_type	= typename nth_type<3, int, long, char>::type;
	using magic_type	= typename nth_type<k(), int, long, char>::type;
	using magic_type1	= typename nth_type<Number().K(), int, long, char>::type;

/*	using no_type = typename nth_type<1>::type;
	using fourth_type = typename nth_type<4, int, long, char>::type;
	using negative_type = typename nth_type<-1, int, long, char>::type;
	using zero_type = typename nth_type<0, int, long, char>::type;
	*/
	
}
