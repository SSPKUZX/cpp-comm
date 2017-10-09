#pragma once

namespace utl 
{
	class Noncopyable
	{
		protected:	
			Noncopyable(){}
//			virtual ~Noncopyable(){}

		public:
			Noncopyable( Noncopyable const&)				= delete;
			Noncopyable& operator= ( Noncopyable const&)	= delete;
	};
}
