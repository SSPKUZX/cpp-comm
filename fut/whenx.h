#pragma once

#include "future.h"

namespace utl{

template <typename... ELEMS> 
struct CollectAllVariadicContext
{
    CollectAllVariadicContext() : m_elems_collected(0) {}
    // Differ from folly: Do nothing here
    ~CollectAllVariadicContext() { }

    CollectAllVariadicContext(const CollectAllVariadicContext& ) = delete;
    void operator= (const CollectAllVariadicContext& ) = delete;
    
    template <typename I, size_t T>
    inline void SetPartialResult(Try<T>& t)
    {
        std::get<I>(m_results) = std::move(t);
		++m_elems_collected;
        if ( m_elems_collected == sizeof...(ELEMS) )
            m_pm.SetValue(std::move(m_results));
    }
        
    
    Promise<std::tuple<Try<ELEMS>...>>	m_pm;
    std::tuple<Try<ELEMS>...>			m_results;
	std::atomic<size_t>					m_elems_collected;		   

    typedef Future<std::tuple<Try<ELEMS>...>> FutureType;
};

// base template
template <template <typename...> class CTX, typename... Ts>
inline void CollectVariadicHelper(const std::shared_ptr<CTX<Ts...>>& ){}
      
template <template <typename ...> class CTX, typename... Ts,
         typename THead, typename... TTail> 
inline void CollectVariadicHelper(const std::shared_ptr<CTX<Ts...>>& ctx, 
     THead&& head, TTail&&... tail)
{
    head.SetCallback([ctx](Try<typename THead::InnerType>&& t) { 
       //  ctx->template SetPartialResult<typename THead::InnerType, 
       //  sizeof...(Ts) - sizeof...(TTail) - 1>(t); 
         ctx->template SetPartialResult<sizeof...(Ts) - sizeof...(TTail) - 1>(std::move(t)); 
        }); 

    CollectVariadicHelper(ctx, std::forward<TTail>(tail)...); 
}
// When All
template <typename... FT>
typename CollectAllVariadicContext<typename std::decay<FT>::type::InnerType...>::FutureType
WhenAll(FT&&... futures)
{
    auto ctx = std::make_shared<CollectAllVariadicContext<typename std::decay<FT>::type::InnerType...>>();

    CollectVariadicHelper<CollectAllVariadicContext>( 
            ctx, std::forward<typename std::decay<FT>::type>(futures)...);

    return ctx->m_pm.GetFuture();
}

// When N 
template <class InputIterator, class T = typename std::iterator_traits<InputIterator>::value_type::InnerType >
Future< std::vector<std::pair<size_t, Try<T>>> > WhenN(size_t N, InputIterator first, InputIterator last){
    size_t nFutures = std::distance(first, last);
    const size_t needCollect = std::min(nFutures, N);
    if (needCollect == 0) {
        return MakeReadyFuture(std::vector<std::pair<size_t, Try<T>>>());
    }

    struct CollectNContext
    {
        CollectNContext(size_t _needs) : needs(_needs) {}
    
        std::mutex mutex;
        const size_t needs;
      //  bool done {false};
        Promise<std::vector<std::pair<size_t, Try<T>>>> pm;
        std::vector<std::pair<size_t, Try<T>>> results;
    };

    auto ctx = std::make_shared<CollectNContext>(needCollect);
    for (size_t i = 0; first != last; ++first, ++i)
    {
        first->SetCallback([ctx, i](Try<T>&& t) {
                std::unique_lock<std::mutex> guard(ctx->mutex);
       //         if (ctx->done)
       //             return;
                ctx->results.push_back(std::make_pair(i, std::move(t)));
                if (ctx->needs == ctx->results.size()) {
               //     ctx->done = true;
                    ctx->pm.SetValue(std::move(ctx->results));
                }
            });
    }
           
    return ctx->pm.GetFuture();
}

template <class InputIterator, typename T = typename std::iterator_traits<InputIterator>::value_type::InnerType>
inline Future< std::vector<Try<T>> > WhenAll(InputIterator first, InputIterator last)
{
	auto n = std::distance(first,last)
	auto fut_pairs = WhenN(n, first, last);
	std::vector<Try<T>> vec_tries(n); 
	for( auto& i2try : fut_pairs.Get()){
		vec_tries[i2try.first]= std::move(i2try.second);	
	}
	return MakeReadyFuture(vec_tries);

 /*   if (first == last)
        return MakeReadyFuture(std::vector<Try<T>>());

    struct CollectAllContext
    {
        CollectAllContext(int n) : results(n) {}
        ~CollectAllContext()
        { 
            // I don't think this line should appear
            // pm.SetValue(std::move(results));
        }
             
        Promise<std::vector<Try<T>>>	pm;
        std::vector<Try<T>>				results;
        std::atomic<size_t>				collected{0};
    };

    auto ctx = std::make_shared<CollectAllContext>(std::distance(first, last));
                
    for (size_t i = 0; first != last; ++first, ++i)
    {
        first->SetCallback([ctx, i](Try<T>&& t) {
                ctx->results[i] = std::move(t); // assignment ???
                if (ctx->results.size() - 1 ==
                    std::atomic_fetch_add (&ctx->collected, std::size_t(1))) {
                    ctx->pm.SetValue(std::move(ctx->results));
                }
            });
    }

    return ctx->pm.GetFuture();
	*/
}

// When Any
template <class InputIterator, class T = typename std::iterator_traits<InputIterator>::value_type::InnerType >
inline Future< std::pair<size_t,Try<T>> > WhenAny(InputIterator first, InputIterator last) {
	return MakeReadyFuture(WhenN(1, first, last).Get()[0]);
/*    if (first == last){
        return MakeReadyFuture(std::make_pair(size_t(0), Try<T>(T())));
    }

    struct CollectAnyContext
    {
        CollectAnyContext() {};
        Promise<std::pair<size_t, Try<T>>> pm;
        std::atomic<bool> done{false};
    };

    auto ctx = std::make_shared<CollectAnyContext>();
    for (size_t i = 0; first != last; ++first, ++i)
    {
        first->SetCallback([ctx, i](Try<T>&& t) {
            if (!ctx->done.exchange(true)) {
                ctx->pm.SetValue(std::make_pair(i, std::move(t)));
            }
       });
    }
           
    return ctx->pm.GetFuture();
*/
}


} // end of utl
