#pragma once

#include <future>
#include <atomic>

#include "helper.h"
#include "try.h"
#include "sched/scheduler.h"

namespace utl 
{

namespace internal
{

enum class Progress
{
    None,
    Timeout,
    Done
};

using TimeoutCallback = std::function<void()>;

template <typename T>
struct State
{
    static_assert(std::is_same<T, void>::value || std::is_copy_constructible<T>(), "must be copyable or void");
    static_assert(std::is_same<T, void>::value || std::is_move_constructible<T>(), "must be movable or void");

    State() : ft_(pm_.get_future()), progress_(Progress::None), retrieved_(false){}

    std::promise<T> pm_;
    std::future<T> ft_;

    std::mutex thenLock_;
    std::function<void (Try<T>&& )> then_;
    Progress progress_;

    std::function<void (TimeoutCallback&& )> onTimeout_;
    std::atomic<bool> retrieved_;

    bool IsRoot() const { return !onTimeout_; }
};

// specialize for void 
template<class... Args>
inline void setState( std::shared_ptr<State<void>>& state_sptr, Args&&... args)
{
	state_sptr->pm_.set_value();
	if(state_sptr->then_){
		state_sptr->then_(Try<void>());
	}
}

// specialize for non-void 
template<class T, class Arg>
inline auto setState( std::shared_ptr<State<T>>& state_sptr, Arg&& arg)
	->typename std::enable_if<!std::is_void<T>::value>::type
{
	state_sptr->pm_.set_value(arg);
	if(state_sptr->then_){
		state_sptr->then_(std::forward<Arg>(arg));
	}
}

} // end namespace internal


template <typename T>
class Future;

template <typename T>
class Promise
{
public:
    Promise() : state_(std::make_shared<internal::State<T>>()){}

    Promise(const Promise&) = delete;
    Promise& operator= (const Promise&) = delete;
	// only move is supported
    Promise(Promise&& pm) = default;
    Promise& operator= (Promise&& pm) = default;

	// added by ravine
    template <typename... Params>
    void SetValue(Params&&... params)
    {
		static_assert(sizeof...(Params)<=1, "Params in SetValue should be less equal one");

        std::unique_lock<std::mutex> guard(state_->thenLock_);
        bool isRoot = state_->IsRoot();
        if (isRoot && state_->progress_ != internal::Progress::None)
            return;

        state_->progress_ = internal::Progress::Done;

		internal::setState(state_, std::forward<Params>(params)... );
    }


    Future<T> GetFuture()
    {
        bool expect = false;
        if (!state_->retrieved_.compare_exchange_strong(expect, true))
        {
            struct FutureAlreadyRetrieved {};
            throw FutureAlreadyRetrieved();
        }

        return Future<T>(state_);
    }

private:
    std::shared_ptr<internal::State<T>> state_;
};

template<class T, class F, class R, class... Args>
inline auto setPM( std::shared_ptr<Try<T>>& value_sptr, F decay_f, std::shared_ptr<Promise<typename R::IsReturnsFuture::Inner>>& pm_sptr)
	-> typename std::enable_if<!R::IsReturnsFuture::value,std::function<void()>>::type
{
	return [value_sptr, decay_f, pm_sptr]() {
		auto result = WrapWithTry(decay_f, value_sptr->template Get<Args>()...);
		pm_sptr->SetValue(std::move(result));
	};
} 

template<class T, class F, class R, class... Args>
inline auto setPM( std::shared_ptr<Try<T>>& value_sptr, F decay_f, std::shared_ptr<Promise<typename R::IsReturnsFuture::Inner>>& pm_sptr)
	-> typename std::enable_if<R::IsReturnsFuture::value,std::function<void()>>::type
{
	return [value_sptr, decay_f, pm_sptr]() {
		// because func return another future:f2, when f2 is done, nextFuture can be done
		auto f2 = decay_f(value_sptr->template Get<Args>()...);
		f2.SetCallback([pm_sptr](Try<typename R::IsReturnsFuture::Inner>&& b) mutable {
			pm_sptr->SetValue(std::move(b));
		});
	};
} 

template <typename T>
class Future
{
public:
    using InnerType = T;

    Future(){}

    Future(const Future&) = delete;
    void operator= (const Future&) = delete;

    Future(Future&& fut) = default;
    Future& operator= (Future&& fut) = default;

    explicit
    Future(std::shared_ptr<internal::State<T>> state) :
        state_(std::move(state)){}

    bool IsReady() const{
        // can be called multi times
        auto res = this->WaitFor(std::chrono::seconds(0));
        return res == std::future_status::ready;
    }

    template <typename SHIT = T>
    typename std::enable_if<!std::is_void<SHIT>::value, T>::type
    GetValue(){
        // only once
        return state_->ft_.get();
    }

    template <typename SHIT = T>
    typename std::enable_if<std::is_void<SHIT>::value, Try<void> >::type
    GetValue(){
        state_->ft_.get();
        return Try<void>();
    }

    template<typename R, typename P>
    std::future_status
    WaitFor(const std::chrono::duration<R, P>& dur) const {
        return state_->ft_.wait_for(dur);
    }

    template<typename Clock, typename Duration>
    std::future_status
    WaitUntil(const std::chrono::time_point<Clock, Duration>& abs) const{
        return state_->ft_.wait_until(abs);
    }

    template <typename F, typename R = internal::CallableResult<F, T> >
    auto Then(F&& f) -> typename R::ReturnFutureType {
        typedef typename R::Arg Arguments;
        return _ThenImpl<F, R>(nullptr, std::forward<F>(f), Arguments());  
    }

    // f will be called in sched
    template <typename F, typename R = internal::CallableResult<F, T> > 
    auto Then(Scheduler* sched_ptr, F&& f) -> typename R::ReturnFutureType 
    {
        typedef typename R::Arg Arguments;
        return _ThenImpl<F, R>(sched_ptr, std::forward<F>(f), Arguments());  
    }

private:
    template <typename F, typename R, typename... Args>
    typename R::ReturnFutureType _ThenImpl(Scheduler* sched, F&& f, std::result_of<F(Args...)> )
    {
        static_assert(std::is_void<T>::value ? sizeof...(Args) == 0 : sizeof...(Args) == 1,
                      "Then callback must take 0/1 argument");

        using FReturnType = typename R::IsReturnsFuture::Inner;

        auto pm_sptr = std::make_shared<Promise<FReturnType>>(); 
		//Promise<FReturnType> pm;
        auto nextFuture = pm_sptr->GetFuture();
        //auto nextFuture = pm.GetFuture();
		// added
		auto decay_f	= static_cast<typename std::decay<F>::type>(f);

        std::unique_lock<std::mutex> guard(state_->thenLock_);
        if (state_->progress_ == internal::Progress::Timeout){
            struct FutureWrongState {};
            throw FutureWrongState();
		} else if (state_->progress_ == internal::Progress::Done){
            guard.unlock();

            auto value_sptr	= std::make_shared<Try<T>>(GetValue()); 
			auto func = setPM<T,F,R,Args...>(value_sptr, decay_f, pm_sptr);  

            if (sched){
                sched->ScheduleOnce(std::move(func)); 
			} else { 
                func();
			}
		} else {
            // 1. set pm's timeout callback
			auto weak_parent = std::weak_ptr<internal::State<T>>(this->state_);
            nextFuture.SetOnTimeout( [weak_parent](internal::TimeoutCallback&& cb) {
                    auto parent = weak_parent.lock();
                    if (!parent)
                        return;

                    {
                        std::unique_lock<std::mutex> guard(parent->thenLock_);
                        if (parent->progress_ != internal::Progress::None)
                            return;
                    
                        parent->progress_ = internal::Progress::Timeout;
                    }

                    if (!parent->IsRoot())
                        parent->onTimeout_(std::move(cb)); // propogate to the root
                    else
                        cb();
                });

            // 2. set this future's then callback
            SetCallback([sched, decay_f, pm_sptr](Try<T>&& value) mutable {

				auto value_sptr = std::make_shared<Try<T>>(std::move(value));
				auto cb = setPM<T,F,R,Args...>(value_sptr, decay_f, pm_sptr);  

                if (sched) {
                    sched->ScheduleOnce(std::move(cb));
				} else {
                    cb();
				}
            });
        }

        return std::move(nextFuture);
    }

public:
	//should be package level
    inline void SetCallback(std::function<void (Try<T>&& )>&& func){
        this->state_->then_ = std::move(func);
    }

	//should be package level
    inline void SetOnTimeout(std::function<void (internal::TimeoutCallback&& )>&& func){
        this->state_->onTimeout_ = std::move(func);
    }

    /*
     * When register callbacks and timeout for a future like this:
     *
     *      Future<int> f;
     *      f.Then(xx).Then(yy).OnTimeout(zz);
     *
     * There will be 3 future objects created except f, we call f as root future.
     * The zz callback is register on the last future, however, timeout and future satisfication
     * can happened almost in the same time, we should ensure that both xx and yy will be called
     * or zz will be called, but they can't happened both or neither. So we pass the cb
     * to the root future, if we find out that root future is indeed timeout, we call cb there.
	 *
	 * ### timeout should be scheduled in different thread compared to future, or timeout will not
	 * ### not work 
     */
    void OnTimeout(std::chrono::milliseconds const& duration,
                   internal::TimeoutCallback f,
                   Scheduler* scheduler)
    {
      //  scheduler->ScheduleOnceAfter(duration, [state = this->state_, cb = std::move(f)]() mutable {
		auto state = state_;
        scheduler->ScheduleOnceAfter(duration, [state, f]() mutable {
                {
                    std::unique_lock<std::mutex> guard(state->thenLock_);

                    if (state->progress_ != internal::Progress::None)
					{
                        return;
					}

                    state->progress_ = internal::Progress::Timeout;
                }

                if (!state->IsRoot()){
                    state->onTimeout_(std::move(f)); // propogate to the root future
				} else {
                    f();
				}
        });
    }

private:
    std::shared_ptr<internal::State<T>> state_;
};

// Make ready future
template <typename T2>
inline Future<T2> MakeReadyFuture(T2&& value){
    Promise<T2> pm;
    auto f(pm.GetFuture());
    pm.SetValue(std::forward<T2>(value));

    return std::move(f);
}

inline Future<void> MakeReadyFuture(){
    Promise<void> pm;
    auto f(pm.GetFuture());
    pm.SetValue();

    return std::move(f);
}

} // end namespace utl 

