#pragma once

#include <assert.h>
#include <chrono>              // NOLINT
#include <condition_variable>  // NOLINT

#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/platform/types.h"

namespace utl{

class Notification {
 public:
  Notification() : m_notified(false) {}
  ~Notification() {}

  void operator()() {
	std::lock_guard<std::mutex> l(m_mu);
	assert(!m_notified);
   	m_notified = true;
   	m_cv.notify_all();
  }

  operator bool() const{
	std::lock_guard<std::mutex> l(m_mutex);
    return m_notified;
  }

  void Wait() {
	std::lock_guard<std::mutex> l(m_mutex);
    while (!m_notified) {
      m_cv.wait(l);
    }
  }

 private:
  friend bool TimedWait(Notification* n, int64 timeout_in_us);

  bool timedWait(int64 timeout_in_us) {
	std::lock_guard<std::mutex> l(m_mutex);
    return m_cv.wait_for(l, std::chrono::microseconds(timeout_in_us),
                        [this]() { return m_notified; });
  }

  mutable std::mutex		m_mutex;
  std::condition_variable	m_cv;
  bool						m_notified;
};

inline bool TimedWait(Notification* n, int64 timeout_in_us) {
  return n->timedWait(timeout_in_us);
}

}  // namespace utl 
