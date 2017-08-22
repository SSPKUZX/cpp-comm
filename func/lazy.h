#include <tuple>
#include <functional>
#include <utility>

namespace lazy
{
	/*template<typename F, size_t... I, typename ... Args>
	inline auto tuple_apply_impl(const F& f, const std::index_sequence<I...>&, const std::tuple<Args...>& tp)
	{
		return f(std::get<I>(tp)...);
	}
	*/

    template<typename F, typename ... Args>
	inline auto tuple_apply(const F& f, const std::tuple<Args...>& tp) -> decltype(f(std::declval<Args>()...))
	{
		const size_t size = std::tuple_size<Args...>::value;
		size_t index = 0;
		auto indexes = std::make_tuple();
		while(index < size)	
		{
			indexes = st::tuple_cat( indexes, std::make_tuple( index++ ) );
		}
		return tuple_apply_impl(f, std::make_index_sequence<sizeof... (Args)>{}, tp);
	}
}
