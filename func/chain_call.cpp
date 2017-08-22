/*
 * reference:http://www.cnblogs.com/qicosmos/p/3521647.html
 * */

#include <functional>
#include <iostream>

template<typename T>
class Task;

template<typename R, typename...Args>
class Task<R(Args...)>
{
	public:
		template<class F>
		Task(F&& f) : m_fn(std::forward<F>(f) ){}

		// run with those args
		R Run(Args&&... args) 
		{ 
			return m_fn(std::forward<Args>(args)...); 
		} 
		
		// take a function which takes result R as input and return R' as parameter
		// return a task which takes Args... as input and return R' 
		template<typename F> 
		auto Then(F&& f)->Task< typename std::result_of<F(R)>::type (Args...)> 
		{ 
			return Task<typename std::result_of<F(R)>::type (Args...)>([this, &f](Args&&... args)
						{
							return f(m_fn(std::forward<Args>(args)...) ); 
						});
		}

		private:
			std::function<R(Args...)> m_fn;
};

int main()
{
	Task<int(int)> task = [](int i){return i;};
	auto result = task.Then([](int i){return std::string("first+");})
					  .Then([](std::string first){return first + "second";})
					  .Then([](std::string second){ std::cout << second << '\n'; return 3;})
					  .Run(1);
	std::cout << "the final result is=" <<  result << '\n';
	
}

