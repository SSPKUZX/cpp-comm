#pragma once

#include "noncopyable.h"

#include <mutex>
#include <string>
#include <stdexcept>
//#include <cstdlib>  // std::atexit
#include <memory>
#include <iostream>

namespace cmn
{
	template<typename Tp, bool defaulted = true>
	class Singleton : private Noncopyable
	{
		public:
			static Tp& Instance()
			{
				// 1>c11 guarantees that a local static variable is only run once
				// this ensures thread-safety in initializing singletion class
				// in concurrecy context
				// 2>it won't init unless it's first accessed
				// 3>if g_instance works as a static member variable
				// it will be inited before main, which means
				// its initialization can't take advantage of 
				// info known only once the program is up and running
				// ### it's unnecessary to be zero-argument default constructed 	
				// ### it should support various constructor
				static Tp s_instance;  

				return s_instance;
			}

		protected:
			// if a class supports singleton, it has to inherit class Singleton
			Singleton(){};
			virtual ~Singleton(){};
	};

	template<typename Tp>
	class Singleton<Tp, false> : private Noncopyable
	{
		public:
			template<class... Args>
				static Tp& Instance(Args&&... args)
				{
					std::call_once( s_inited_once_flag, [&](){ 
							s_pInstance = std::unique_ptr<Tp>( new Tp(std::forward<Args>(args)... ) );
						//	std::atexit(destroy); 
							});	
					return *s_pInstance;
				}

			static Tp& Instance()
			{
				if( nullptr == s_pInstance )				
				{
					throw std::logic_error(std::string() + typeid(Tp).name() + " is not inited!\n");
				}
				return *s_pInstance;	
			}

		protected:
			// if a class supports singleton, it has to inherit class Singleton
			Singleton(){};
			virtual ~Singleton(){};

		private:
			static std::unique_ptr<Tp>	s_pInstance;
			static std::once_flag		s_inited_once_flag;

		/*	static void destroy()
			{
				if( nullptr != s_pInstance )	
				{
					delete s_pInstance;	
					s_pInstance = nullptr;
					std::cout << typeid(Tp).name() << " get deleted in " << __PRETTY_FUNCTION__ << '\n';
				}
			}
			*/
	};

	template<class Tp>
	std::unique_ptr<Tp>  Singleton<Tp, false>::s_pInstance;

	template<class Tp>
	std::once_flag Singleton<Tp, false>::s_inited_once_flag;
}
