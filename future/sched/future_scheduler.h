#pragma once 

#include "scheduler.h"
#include "future_message.h"
//#include "future/future.h"
#include "cmf/poster.h"
//#include "cmf/regional_office.h"

namespace utl{
	
	class FutureScheduler final : public Scheduler{ //, public cmf::RegionalOffice{
		public:		
			FutureScheduler(cmf::Poster poster = cmf::Poster()) : Scheduler(), m_poster(poster){}//RegionalOffice(), 
			~FutureScheduler(){}

			// ensure that even duration  
			inline virtual void ScheduleOnceAfter(cmf::ms const& duration, std::function<void()> functor) override{
				if(m_poster){
					m_poster(cmf::make_message<FutureMessage>(std::move(functor))->AriseAfter(duration));	
				} else {
					throw std::logic_error("ScheduleOnceAfer is not supported now b' current poster is invalid");	
				}
			}
			inline virtual void ScheduleOnce(std::function<void()> functor) override{
				if(m_poster){
					m_poster(cmf::make_message<FutureMessage>( std::move(functor)) );	
				} else{
					functor();	
				}
			}
/*
			inline virtual void operator()(const sptr<Message>& msg ) override{
				auto wrapped_message = dynamic_cast<WrappedMessage<FutureMessage>*>(msg.get());
				auto future_message = static_cast<FutureMessage const&>(*wrapped_message); 
				future_message();
			}

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
