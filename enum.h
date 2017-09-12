#include "variadic.h"
#include "make_unique.h"
#include "demangle.h"

namespace utl 
{
	template<class... Labels>
	class Enum
	{
		static_assert( !has_duplicate<Labels...>::value, "no duplicate label classes allowed in Enum's template class arguments");
		public:	
			// similar to equal
			template<class T>
			bool Is() const
			{ 
				static_assert(::utl::one_of<T,Labels...>::value,"T must be one of Labels");
				return m_index==utl::index_of<T, Labels...>::value; 
			}
		
			// tostring
			std::string const& Str() const{ return meta.s_label_names[m_index]; }

			// system enum can be converted to int
			operator int() const{ return m_index;}

			// total label count
			constexpr uint8_t Count() const { return sizeof...(Labels); } 
	
			// factory method 
			template<class T>
			static Enum Of()
			{ 
				static_assert(::utl::one_of<T,Labels...>::value,"T must be one of Labels");
				return Enum(utl::index_of<T, Labels...>::value);
			}

			template<class... _Labels>
			friend std::ostream& operator<<( std::ostream&, Enum<_Labels...> const& val);

		private:	
			// only accessible to 'Of'
			Enum( uint8_t const idx) : m_index(idx){}
	
			uint8_t	m_index;
	
			// static initor of member s_label_names
			static struct MetaHolder
			{
				std::unique_ptr<std::string[]>	s_label_names;
	
				template<class T>
				struct EnumHook 
				{
					void operator()( std::unique_ptr<std::string[]>& names, uint8_t& index)	
					{
						names[index++] = utl::demangle(typeid(T));	
					}
				};
				
				MetaHolder() : s_label_names(::cmn::make_unique<std::string[]>(sizeof...(Labels)))
				{
					uint8_t index = 0;
					utl::detail::variadic_np<EnumHook, Labels...>()( s_label_names, index);
				}
			} meta;
	};
	
	template<class... Labels >
	typename Enum<Labels...>::MetaHolder	Enum<Labels...>::meta;
	
	template<class... _Labels>
	std::ostream& operator<< ( std::ostream& os, Enum<_Labels...> const& val)
	{
		os << Enum<_Labels...>::meta.s_label_names[val.m_index];
		return os; 
	}
} // end of cmn

namespace std
{
	template<class... Types>
	struct hash<::cmn::Enum<Types...>>
	{
		std::hash<int> hasher;		
		size_t operator()( ::cmn::Enum<Types...> const& e) const
		{
			return hasher(static_cast<int>(e));	
		}
	};
}
