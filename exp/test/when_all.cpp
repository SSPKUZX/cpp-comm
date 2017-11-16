#include <future> // future,this_thread
#include <vector>
#include <iostream> // cout, 
#include <algorithm> // transform,back_inserter,ostream_iterator


template<class T>
std::vector<T> WhenAll(std::vector<std::future<T>> futures ){
	std::vector<T>	results;
	std::transform(futures.begin(), futures.end(), std::back_inserter(results), 
				[](std::future<T>& fut){ return fut.get(); }
			);
	return std::move(results);
} 

/* begin/end of initializer_list is const, so {fut.get()} can't invoke
template<class T>
std::vector<T> WhenAll(std::initializer_list<std::future<T>> futures ){
	std::vector<T>	results;
	std::transform(futures.begin(), futures.end(), std::back_inserter(results), 
				[](std::future<T> const& fut) { return fut.get(); }
			);
	return std::move(results);
} 
*/

template<class... Ts>
struct same_types : std::false_type{};

template<class T>
struct same_types<T> : std::true_type{};

template<class T, class U, class... Ts>
struct same_types<T, U, Ts...> : std::integral_constant<bool, 
		std::is_same<T,U>::value && same_types<U, Ts...>::value>{};

template<class... Args>
auto WhenAll( std::future<Args>&... futs )
	-> typename std::enable_if< not same_types<Args...>::value, 
								std::tuple<Args...> >::type{
	return std::make_tuple( futs.get()... );
}

template<class... Ts>
auto WhenAll( std::future<Ts>&... futs)
	-> typename std::enable_if<same_types<Ts...>::value, 
	std::vector<typename std::common_type<Ts...>::type> >::type{
	return {futs.get()...};
}

int main(){
	auto fut1 = std::async([](){
				std::this_thread::sleep_for(std::chrono::milliseconds(300));			
				return 4;
			});
	auto fut2 = std::async([](){
				std::this_thread::sleep_for(std::chrono::milliseconds(400));			
				return 5;
			});
	auto fut3 = std::async([](){
				std::this_thread::sleep_for(std::chrono::milliseconds(200));			
				return 8;
			});

	if(false){
	
		std::vector<std::future<int>> futures;
		futures.push_back(std::move(fut1));
		futures.push_back(std::move(fut2));
		futures.push_back(std::move(fut3));
		auto results = WhenAll(std::move(futures));
	//	auto results = WhenAll<int>({std::move(fut1), std::move(fut2), std::move(fut3)});
		std::copy(results.begin(), results.end(), std::ostream_iterator<int>(std::cout, "\t") );
		std::cout << std::endl;
	} else {
		auto results = WhenAll( fut1, fut2, fut3);
		std::copy(results.begin(), results.end(), std::ostream_iterator<int>(std::cout, "\t") );
		std::cout << std::endl;
//		std::cout << std::get<0>(tup) << '\t' << std::get<1>(tup) << '\t' << std::get<2>(tup) << '\n';
	}

	return 0;
}
