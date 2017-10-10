#pragma once

#include <chrono>              
#include <condition_variable>  


namespace utl{

	class Notification {
	 public:
	  Notification() : m_notified(false) {}
	  ~Notification() {}
	
	  // do notify 
	  void operator()() {
		std::lock_guard<std::mutex> l(m_mutex);
	   	m_notified = true;
	   	m_cv.notify_all();
	  }
	
	  // has ever notified
	  operator bool() const{
		std::lock_guard<std::mutex> l(m_mutex);
	    return m_notified;
	  }
	
	  void Wait() {
		std::unique_lock<std::mutex> l(m_mutex);
	    while (!m_notified) {
	      m_cv.wait(l);
	    }
	  }
	
	  template<class Duration>
	  bool Wait(Duration const& d) {
		std::unique_lock<std::mutex> l(m_mutex);
	    return m_cv.wait_for(l, d,
	                        [this]() { return m_notified; });
	  }
	
	 private:
	
	  mutable std::mutex		m_mutex;
	  std::condition_variable	m_cv;
	  bool						m_notified;
	};

}  // namespace utl 
