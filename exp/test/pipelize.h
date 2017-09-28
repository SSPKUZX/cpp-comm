/*
 * 
 * pipelize the data flow like this
 *  numbers | map([](x){ return f(x);}) | log();
 * numbers could be one of SeqContainers -> deque&list&vector
 * */
#pragma once


namespace utl 
{
	    
	template<class T, class F>
	inline auto operator|(T&& x, const F& f) -> decltype(f(std::forward<T>(x)))
	{
		return f( std::forward<T>(x) );
	}
     
/*
	template <typename T>
	T whereInList(const T& list, std::function<bool(typename T::value_type)> f)
	{
	   	 T result;
	   	 for (auto& item : list){
			if (f(item)){
				result.push_back(item);
			}
		 }
	   	 return move(result);
	}
     
	template <typename T>
	T mapToList(const T& list, std::function< typename T::value_type( typename T::value_type  )> f)
	{
		T result;
	    for (auto& item : list){
			result.push_back(f(item));
		}
		return move(result);
	}
	template <typename T>
	T logList(const T& list)
	{
		std::cout << "List: ";
	    for (auto& item : list){
		   std::cout << item << " ";
		}
		std::cout << std::endl;
		return list;
	}
 */    

	template <class F>
	struct Pipeable
	{
		private:
			F f;
		public:
		    Pipeable(F&& f) : f(std::forward<F>(f)) {}
			     
			template<class... Xs>
			auto operator()(Xs&&... xs) -> decltype(std::bind(f, std::placeholders::_1, std::forward<Xs>(xs)...)) const {
				return std::bind(f, std::placeholders::_1, std::forward<Xs>(xs)...);
			}
	};
	    
	template <class F>
	Pipeable<F> pipelize(F&& f) { return Pipeable<F>{std::forward<F>(f)}; }

/*	template <typename F>
	auto piped1Arg(F f) -> decltype(pipelize(std::bind(f, placeholders::_1)))
	{
		return pipelize(std::bind(f, placeholders::_1));
	}
     
     
	template <typename F>
	auto piped2Args(F f) -> decltype(pipelize(std::bind(f, placeholders::_1, placeholders::_2)))
	{
		return pipelize(std::bind(f, placeholders::_1, placeholders::_2));
	}
*/
}
