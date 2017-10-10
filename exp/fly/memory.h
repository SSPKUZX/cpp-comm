/*
 * Copyright 2017 Facebook, Inc.
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

#pragma once

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

namespace utl {

/**
 * For exception safety and consistency with make_shared. Erase me when
 * we have std::make_unique().
 *
 * @author Louis Brandy (ldbrandy@fb.com)
 * @author Xu Ning (xning@fb.com)
 */

#if __cplusplus >= 201402L || __cpp_lib_make_unique >= 201304L || \
    (__ANDROID__ && __cplusplus >= 201300L) || _MSC_VER >= 1900

/* using override */ using std::make_unique;

#else

template <typename T, typename... Args>
typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T>>::type
make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// Allows 'make_unique<T[]>(10)'. (N3690 s20.9.1.4 p3-4)
template <typename T>
typename std::enable_if<std::is_array<T>::value, std::unique_ptr<T>>::type
make_unique(const size_t n) {
  return std::unique_ptr<T>(new typename std::remove_extent<T>::type[n]());
}

// Disallows 'make_unique<T[10]>()'. (N3690 s20.9.1.4 p5)
template <typename T, typename... Args>
typename std::enable_if<
  std::extent<T>::value != 0, std::unique_ptr<T>>::type
make_unique(Args&&...) = delete;

#endif

/**
 * static_function_deleter
 *
 * So you can write this:
 *
 *      using RSA_deleter = folly::static_function_deleter<RSA, &RSA_free>;
 *      auto rsa = std::unique_ptr<RSA, RSA_deleter>(RSA_new());
 *      RSA_generate_key_ex(rsa.get(), bits, exponent, nullptr);
 *      rsa = nullptr;  // calls RSA_free(rsa.get())
 *
 * This would be sweet as well for BIO, but unfortunately BIO_free has signature
 * int(BIO*) while we require signature void(BIO*). So you would need to make a
 * wrapper for it:
 *
 *      inline void BIO_free_fb(BIO* bio) { CHECK_EQ(1, BIO_free(bio)); }
 *      using BIO_deleter = folly::static_function_deleter<BIO, &BIO_free_fb>;
 *      auto buf = std::unique_ptr<BIO, BIO_deleter>(BIO_new(BIO_s_mem()));
 *      buf = nullptr;  // calls BIO_free(buf.get())
 */
/*
template <typename T, void(*f)(T*)>
struct static_function_deleter {
  void operator()(T* t) const {
    f(t);
  }
};
*/
// no need to specify T again
template<class FPtr>
struct static_function_deleter; 
template <typename T>
struct static_function_deleter<void(*f)(T*)> {
  void operator()(T* t) const {
    f(t);
  }
};

/**
 *  to_shared_ptr
 *
 *  Convert unique_ptr to shared_ptr without specifying the template type
 *  parameter and letting the compiler deduce it.
 *
 *  So you can write this:
 *
 *      auto sptr = to_shared_ptr(getSomethingUnique<T>());
 *
 *  Instead of this:
 *
 *      auto sptr = shared_ptr<T>(getSomethingUnique<T>());
 *
 *  Useful when `T` is long, such as:
 *
 *      using T = foobar::FooBarAsyncClient;
 */
template <typename T, typename D>
inline std::shared_ptr<T> to_shared_ptr(std::unique_ptr<T, D>&& ptr) {
  return std::shared_ptr<T>(std::move(ptr));
}

/**
 *  to_weak_ptr
 *
 *  Make a weak_ptr and return it from a shared_ptr without specifying the
 *  template type parameter and letting the compiler deduce it.
 *
 *  So you can write this:
 *
 *      auto wptr = to_weak_ptr(getSomethingShared<T>());
 *
 *  Instead of this:
 *
 *      auto wptr = weak_ptr<T>(getSomethingShared<T>());
 *
 *  Useful when `T` is long, such as:
 *
 *      using T = foobar::FooBarAsyncClient;
 */
template <typename T>
inline std::weak_ptr<T> to_weak_ptr(const std::shared_ptr<T>& ptr) {
  return std::weak_ptr<T>(ptr);
}

namespace detail {
/**
 * Not all STL implementations define ::free in a way that its address can be
 * determined at compile time. So we must wrap ::free in a function whose
 * address can be determined.
 */
inline void SysFree(void* p) {
  ::free(p);
}
}

//using SysBufferDeleter = static_function_deleter<void, &detail::SysFree>;
using SysBufferDeleter = static_function_deleter<&detail::SysFree>;
using SysBufferUniquePtr = std::unique_ptr<void, SysBufferDeleter>;
inline SysBufferUniquePtr allocate_sys_buffer(size_t size) {
  return SysBufferUniquePtr(::malloc(size));
}


/*
 * folly::enable_shared_from_this
 *
 * To be removed once C++17 becomes a minimum requirement for folly.
 */
#if __cplusplus >= 201700L || \
    __cpp_lib_enable_shared_from_this >= 201603L

// Guaranteed to have std::enable_shared_from_this::weak_from_this(). Prefer
// type alias over our own class.
/* using override */ using std::enable_shared_from_this;

#else

/**
 * Extends std::enabled_shared_from_this. Offers weak_from_this() to pre-C++17
 * code. Use as drop-in replacement for std::enable_shared_from_this.
 *
 * C++14 has no direct means of creating a std::weak_ptr, one must always
 * create a (temporary) std::shared_ptr first. C++17 adds weak_from_this() to
 * std::enable_shared_from_this to avoid that overhead. Alas code that must
 * compile under different language versions cannot call
 * std::enable_shared_from_this::weak_from_this() directly. Hence this class.
 *
 * @example
 *   class MyClass : public folly::enable_shared_from_this<MyClass> {};
 *
 *   int main() {
 *     std::shared_ptr<MyClass> sp = std::make_shared<MyClass>();
 *     std::weak_ptr<MyClass> wp = sp->weak_from_this();
 *   }
 */
template <typename T>
class enable_shared_from_this : public std::enable_shared_from_this<T> {
 public:
  constexpr enable_shared_from_this() noexcept = default;

  std::weak_ptr<T> weak_from_this() noexcept {
    return weak_from_this_<T>(this);
  }

  std::weak_ptr<T const> weak_from_this() const noexcept {
    return weak_from_this_<T>(this);
  }

 private:
  // Uses SFINAE to detect and call
  // std::enable_shared_from_this<T>::weak_from_this() if available. Falls
  // back to std::enable_shared_from_this<T>::shared_from_this() otherwise.
  template <typename U>
  auto weak_from_this_(std::enable_shared_from_this<U>* base_ptr)
  noexcept -> decltype(base_ptr->weak_from_this()) {
    return base_ptr->weak_from_this();
  }

  template <typename U>
  auto weak_from_this_(std::enable_shared_from_this<U> const* base_ptr)
  const noexcept -> decltype(base_ptr->weak_from_this()) {
    return base_ptr->weak_from_this();
  }

  template <typename U>
  std::weak_ptr<U> weak_from_this_(...) noexcept {
    try {
      return this->shared_from_this();
    } catch (std::bad_weak_ptr const&) {
      // C++17 requires that weak_from_this() on an object not owned by a
      // shared_ptr returns an empty weak_ptr. Sadly, in C++14,
      // shared_from_this() on such an object is undefined behavior, and there
      // is nothing we can do to detect and handle the situation in a portable
      // manner. But in case a compiler is nice enough to implement C++17
      // semantics of shared_from_this() and throws a bad_weak_ptr, we catch it
      // and return an empty weak_ptr.
      return std::weak_ptr<U>{};
    }
  }

  template <typename U>
  std::weak_ptr<U const> weak_from_this_(...) const noexcept {
    try {
      return this->shared_from_this();
    } catch (std::bad_weak_ptr const&) {
      return std::weak_ptr<U const>{};
    }
  }
};

#endif

} // namespace folly
