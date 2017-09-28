#include <future>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>

#include "noncopyable.h"
#include "ptr.h"

namespace thd
{
	class Runnable 	
	{
		public:	
			virtual void Run() = 0;
			virtual ~Runnable(){};
	};

	template<class RetType>
	class RunnableWrapper final : public Runnable, private ::cmn::Noncopyable
	{
		public:
			explicit RunnableWrapper( std::packaged_task<RetType()>&& task) : m_task(std::move(task)){}
			~RunnableWrapper(){}

			inline virtual void Run() override{
				m_task();	
			}

		private:
			std::packaged_task<RetType()>	m_task;
	};

	template<class FuncType, class RetType = typename std::result_of<FuncType()>::type>
	inline std::packaged_task<RetType()> make_task(FuncType&& func)
	{
		return std::packaged_task<RetType()>(std::forward<FuncType>(func));	
	}

	template<class RetType>
	inline std::unique_ptr<Runnable> make_runnable(std::packaged_task<RetType()>&& task)
	{
		return ::cmn::make_unique<RunnableWrapper<RetType>>(std::move(task));	
	}

	class ThreadPool final: private ::cmn::Noncopyable
	{
		public:
			ThreadPool( const size_t pool_size= 3) : m_done(false), m_pool_size(pool_size)
			{ m_pool.reserve(pool_size); }
			~ThreadPool(){
				for( auto& thd : m_pool)
					thd.join();	
			}

			// begin running
			void Run()
			{
				size_t idx =0;	
				while( idx++ < m_pool_size) {
					m_pool.emplace_back( &ThreadPool::doWork, this);	
				}
			}
			// stop running
			inline void Interrupt() {
				m_done = true;	
			}

			// scheduling task
			template<class Task, class ResultType=typename std::result_of<Task()>::type>
			std::future<ResultType> Schedule( Task&& task)
			{
				auto pkg_task = make_task( std::forward<Task>(task));	
				auto fut = pkg_task.get_future();
				m_task_queue.push(make_runnable(std::move(pkg_task)));
				
				return std::move(fut);
			}

		private:
			using Task = std::unique_ptr<Runnable>;
			std::mutex					m_queue_mutex;
			std::queue<Task>			m_task_queue;

			std::atomic<bool>			m_done;
			uint32_t					m_pool_size;
			std::vector<std::thread>	m_pool;	

			void doWork()
			{
				while(!m_done) {	
					try{
						Task task;
						{
							std::lock_guard<std::mutex> lg(m_queue_mutex);
							if(m_task_queue.empty()) {
						//		std::this_thread::yield();		
								std::this_thread::sleep_for(std::chrono::milliseconds(100));
							} else {
								task = std::move(m_task_queue.front());	
								m_task_queue.pop();
							}
						}
						if(task) task->Run();
					}catch(...){
						m_done = true;
						throw;
					}
				}
			}
	};
}
