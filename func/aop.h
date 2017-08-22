#include <functional>
#include <chrono>
#include <string>
#include <iostream>

namespace aop
{
	template <typename R, typename ...Args>
	std::function<R(Args...)> logged(std::string name, std::function<R(Args...)> f)
	{
		return [f,name](Args... args){
			std::cout << name << " start" << std::endl;
			auto start = std::chrono::high_resolution_clock::now();
			R result = f(std::forward<Args>(args)...);
			auto end = std::chrono::high_resolution_clock::now();
			auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			std::cout << "Elapsed: " << total << "us" << std::endl;
			return result;
		};
	}

//	template <typename R, typename ...Args>
//	std::function<Maybe<R>(Args...)> triesTwice(std::function<Maybe<R>(Args...)> f)
//	{
//		return [f](Args... args){
//			Maybe<R> result = f(std::forward<Args>(args)...);
//			if (result.hasError())
//				return f(std::forward<Args>(args)...);
//			return result;
//		};
//	}
//
//	template <typename R, typename C, typename K, typename ...Args>
//	std::function<Maybe<R>(K,Args...)> cached(C & cache, std::function<Maybe<R>(K,Args...)> f)
//	{
//		return [f,&cache](K key, Args... args){
//			// get key as first argument
//		             
//			if (cache.count(key) > 0)
//				return just(cache[key]);
//		    else
//		    {
//				Maybe<R> result = f(std::forward<K>(key), std::forward<Args>(args)...);
//				if (!result.hasError())
//					cache.insert(std::pair<int, R>(key, result())); //add to cache
//		        return result;
//		    }
//	    };
//	}

}
