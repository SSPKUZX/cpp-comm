#ifndef BERT_HELPER_H
#define BERT_HELPER_H

/*
 * This file is modified from facebook folly, with my annotation
 */

/*
 * Copyright 2016 Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <tuple>
#include <vector>
#include <memory>

namespace utl 
{

template <typename T>
class Future;

template <typename T>
class Promise;

template <typename T>
class Try;

namespace internal
{

template<typename Signature>
using result_of_t = typename ::std::result_of<Signature>::type;

// Test if F can be called with Args type
template<typename F, typename... Args>        
struct CanCallWith
{
    // SFINAE  Check
    template<typename T,
        typename Dummy = result_of_t<T(Args...)>>
        static constexpr std::true_type
        Check(std::nullptr_t dummy)
        {
            return std::true_type{};
        };  
                         
    template<typename Dummy>
        static constexpr std::false_type
        Check(...)
        {
            return std::false_type{};
        };
                                
    typedef decltype(Check<F>(nullptr)) type; // true_type if T可以接受Args作为参数        
    static constexpr bool value = type::value; // the integral_constant's value            
};              

// simple traits
template <typename T>
struct IsFuture : std::false_type
{ 
    using Inner = T;
};

template <typename T> 
struct IsFuture<Future<T>> : std::true_type
{ 
    typedef T Inner; 
};

template<typename F, typename T>
struct CallableResult
{ 
    // Test F call with arg type: void, T&&, T&, but do Not choose Try type as args
    typedef
        typename std::conditional<
                 CanCallWith<F>::value, // if true, F can call with void
                 std::result_of<F()>,
                 typename std::conditional< // NO, F(void) is invalid
                          CanCallWith<F, T&&>::value, // if true, F(T&&) is valid
                          std::result_of<F(T&&)>, // Yes, F(T&&) is ok
                          std::result_of<F(T&)> >::type>::type Arg;  // Resort to F(T&)

    // If ReturnsFuture::value is true, F returns another future type.
    typedef IsFuture<typename Arg::type> IsReturnsFuture; 

    // Future callback's result must be wrapped in another future
    typedef Future<typename IsReturnsFuture::Inner> ReturnFutureType;
};


// CallableResult specilization for void.
// I don't know why folly works without this...
template<typename F>
struct CallableResult<F, void>
{ 
    // Test F call with arg type: void
    typedef std::result_of<F()> Arg;

    // If ReturnsFuture::value is true, F returns another future type.
    typedef IsFuture<typename Arg::type> IsReturnsFuture; 

    // Future callback's result must be wrapped in another future
    typedef Future<typename IsReturnsFuture::Inner> ReturnFutureType;
};

// For when_all
//
template <typename... ELEM> 
struct CollectAllVariadicContext
{
    CollectAllVariadicContext() {}
   
    // Differ from folly: Do nothing here
    ~CollectAllVariadicContext() { }

    CollectAllVariadicContext(const CollectAllVariadicContext& ) = delete;
    void operator= (const CollectAllVariadicContext& ) = delete;
    
    template <typename T, size_t I>
    inline void SetPartialResult(Try<T>& t)
    {
        std::get<I>(results) = std::move(t);
        collects.push_back(I);
        if (collects.size() == std::tuple_size<decltype(results)>::value)
            pm.SetValue(std::move(results));
    }
        
    
    Promise<std::tuple<Try<ELEM>...>> pm;
    std::tuple<Try<ELEM>...> results;
    std::vector<size_t> collects;
    
    typedef Future<std::tuple<Try<ELEM>...>> FutureType;
};

// base template
template <template <typename...> class CTX, typename... Ts>
void CollectVariadicHelper(const std::shared_ptr<CTX<Ts...>>& )
{
}
      
template <template <typename ...> class CTX, typename... Ts,
         typename THead, typename... TTail> 
void CollectVariadicHelper(const std::shared_ptr<CTX<Ts...>>& ctx, 
     THead&& head, TTail&&... tail)
{
    head.SetCallback([ctx](Try<typename THead::InnerType>&& t) { 
         ctx->template SetPartialResult<typename THead::InnerType, 
         sizeof...(Ts) - sizeof...(TTail) - 1>(t); 
         }); 

    CollectVariadicHelper(ctx, std::forward<TTail>(tail)...); 
}


} // end namespace internal

} // end namespace ananas

#endif

