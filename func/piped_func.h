#include <functional>

namespace func
{
	template <class F>
	struct pipeable
	{
		private:
			F f;
		public:
		    pipeable(F&& f) : f(std::forward<F>(f)) {}
			     
			template<class... Xs>
			auto operator()(Xs&&... xs) -> decltype(std::bind(f, std::placeholders::_1, std::forward<Xs>(xs)...)) const {
				return std::bind(f, std::placeholders::_1, std::forward<Xs>(xs)...);
			}
	};
	    
	template <class F>
	pipeable<F> piped(F&& f) { return pipeable<F>{std::forward<F>(f)}; }
	    
	template<class T, class F>
	auto operator|(T&& x, const F& f) -> decltype(f(std::forward<T>(x)))
	{
		return f( std::forward<T>(x) );
	}

	using namespace std;

	template <typename T>
	T whereInList(const T& list, std::function<bool(typename T::value_type)> f)
	{
	   	 T result;
	   	 for (auto& item : list)
			if (f(item))
				result.push_back(item);
	   	 return result;
	}
     
	template <typename T>
	T mapToList(const T& list, std::function< typename T::value_type( typename T::value_type  )> f)
	{
		T result;
	    for (auto& item : list)
			result.push_back(f(item));
		return result;
	}
     
	template <typename T>
	T logList(const T& list)
	{
		std::cout << "List: ";
	    for (auto& item : list)
		   std::cout << item << " ";
		std::cout << std::endl;
		return list;
	}
     
     
	template <typename F>
	auto piped1Arg(F f) -> decltype(piped(std::bind(f, placeholders::_1)))
	{
		return piped(std::bind(f, placeholders::_1));
	}
     
     
	template <typename F>
	auto piped2Args(F f) -> decltype(piped(std::bind(f, placeholders::_1, placeholders::_2)))
	{
		return piped(std::bind(f, placeholders::_1, placeholders::_2));
	}

}
