#pragma once 

#include "scheduler.h"
#include "cmf/poster.h"

namespace utl{
	
	class FutureScheduler final : public Scheduler{ 
		public:		
			FutureScheduler(cmf::Poster poster = cmf::Poster()) : Scheduler(), m_poster(poster){}
			~FutureScheduler(){}

			// ensure that even duration  
			inline virtual void ScheduleOnceAfter(cmf::ms const& duration, std::function<void()> functor) override{
				if(m_poster){
					m_poster(cmf::make_message<cmf::FunctionMessage>(std::move(functor))->AriseAfter(duration));	
				} else {
					throw std::logic_error("ScheduleOnceAfer is not supported now b' current poster is invalid");	
				}
			}
			inline virtual void ScheduleOnce(std::function<void()> functor) override{
				if(m_poster){
					m_poster(cmf::make_message<cmf::FunctionMessage>( std::move(functor)) );	
				} else{
					functor();	
				}
			}
/*
		protected:	
			template<class Tp, class... Args>
			inline Future<T> makeReadyFuture( Args&&... args){
				return MakeReadyFuture<Tp>(this, std::forward<Args>(args)... );	
			}
*/
		private:
			cmf::Poster	m_poster;	
	};

}
