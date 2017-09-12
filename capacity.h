#pragma once

#include <type_traits>
#include <cstdint>
#include <string>

/*
 * used to measure disk or memory capacity
 * */
namespace utl
{
	static constexpr const char* s_units[]={"Bit","Byte","KB","MB",
										  "GB", "TB","PB"};

	template<class Rep, uint8_t Bits>
	class Capacity{
		static_assert(std::is_integral<Rep>::value,
				     "Rep in Capacity must be integral");
		public: 
			using RepType	= Rep;
			static const uint8_t BitCount	= Bits;
			Capacity(Rep count) : m_count(count){}
			operator Rep() { return m_count; }
			operator std::string() { 
				return std::to_string(m_count)+s_units[Bits==0?0:Bits/10+1]; 
			}
			Rep Count() const{ return m_count; }	
			Capacity& SetCount(Rep count) {
				m_count = count;
				return *this;
			}

		private:
			Rep	m_count;
	};
	

	template<class CapacityTo, class RepFrom, uint8_t BitsFrom>
	inline auto capacity_cast( Capacity<RepFrom, BitsFrom> const& from)
	-> typename std::enable_if<(CapacityTo::BitCount > BitsFrom), CapacityTo >::type
	{
		return CapacityTo(from.Count()>>(CapacityTo::BitCount-BitsFrom));	
	}

	template<class CapacityTo, class RepFrom, uint8_t BitsFrom>
	inline auto capacity_cast( Capacity<RepFrom, BitsFrom> const& from)
	-> typename std::enable_if<(CapacityTo::BitCount <= BitsFrom), CapacityTo >::type
	{
		return CapacityTo(from.Count()<<(BitsFrom-CapacityTo::BitCount));	
	}

	template<class Rep1, uint8_t Bits1, class Rep2, uint8_t Bits2>
	inline auto  operator+( Capacity<Rep1, Bits1> const& lhs, Capacity<Rep2,Bits2> const& rhs)
	-> typename std::enable_if<(Bits1>Bits2), Capacity<Rep2,Bits2>>::type
	{
		return Capacity<Rep2,Bits2>((lhs.Count()<<(Bits1-Bits2)) + rhs.Count());
	}

	template<class Rep1, uint8_t Bits1, class Rep2, uint8_t Bits2>
	inline auto  operator+( Capacity<Rep1, Bits1> const& lhs, Capacity<Rep2,Bits2> const& rhs)
	-> typename std::enable_if<(Bits1<=Bits2), Capacity<Rep1,Bits1>>::type
	{
		return rhs+lhs; 
	}

	// only allow non-precision-loss addition 
	template<class Rep1, uint8_t Bits1, class Rep2, uint8_t Bits2>
	inline auto  operator+=( Capacity<Rep1, Bits1>& lhs, Capacity<Rep2,Bits2> const& rhs)
	-> typename std::enable_if<(Bits1<=Bits2), Capacity<Rep1,Bits1>&>::type
	{
		return lhs.SetCount( lhs.Count()+(rhs.Count()<<(Bits2-Bits1)) ); 
	}

	template<class Rep1, uint8_t Bits1, class Rep2, uint8_t Bits2>
	inline auto  operator-( Capacity<Rep1, Bits1> const& lhs, Capacity<Rep2,Bits2> const& rhs)
	-> typename std::enable_if<(Bits1>Bits2), Capacity<Rep2,Bits2>>::type
	{
		return Capacity<Rep2,Bits2>((lhs.Count()<<(Bits1-Bits2)) - rhs.Count());
	}

	template<class Rep1, uint8_t Bits1, class Rep2, uint8_t Bits2>
	inline auto  operator-( Capacity<Rep1, Bits1> const& lhs, Capacity<Rep2,Bits2> const& rhs)
	-> typename std::enable_if<(Bits1<=Bits2), Capacity<Rep1,Bits1>>::type
	{
		return Capacity<Rep1,Bits1>( lhs.Count() - (rhs.Count()<<(Bits2-Bits1)) );
	}

	// only allow non-precision-loss addition 
	template<class Rep1, uint8_t Bits1, class Rep2, uint8_t Bits2>
	inline auto  operator-=( Capacity<Rep1, Bits1>& lhs, Capacity<Rep2,Bits2> const& rhs)
	-> typename std::enable_if<(Bits1<=Bits2), Capacity<Rep1,Bits1>&>::type
	{
		return lhs.SetCount( lhs.Count()-(rhs.Count()<<(Bits2-Bits1)) ); 
	}

	template<class Rep, uint8_t Bits, class Multipier>
	Capacity<Rep, Bits>  operator*( Capacity<Rep, Bits> const& lhs, Multipier mlp)
	{
		return Capacity<Rep, Bits>(lhs.Count()*mlp);
	}

	template<class Rep, uint8_t Bits, class Multipier>
	Capacity<Rep, Bits>&  operator*=( Capacity<Rep, Bits>& lhs, Multipier mlp)
	{
		return lhs.SetCount(lhs.Count()*mlp);
	}

	using Bit	= Capacity<int64_t, 0	>;
	using Byte	= Capacity<int64_t, 3	>;
	using KB	= Capacity<int64_t, 13	>;
	using MB	= Capacity<int64_t, 23	>;
	using GB	= Capacity<int32_t, 33	>;
	using TB	= Capacity<int32_t, 43	>;
	using PB	= Capacity<int16_t, 53	>; 
}
