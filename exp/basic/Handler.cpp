#include "Handler.h"

templalte<typename T>
Handler::Handler(T *t)
{
	t = t;
	count = new int(1);
}

template<typename T>
Handler::Handler(const Handlder& rhs)
{
	t = rhs.t;
	count = rhs.count;
	++*count;
}

template<class T>
void Handler::rm_ref()
{
	if(--*count == 0)
	{
		delete t; 
		t = 0;
		delete count;
		count = 0;
	}
}

templalte<typename T>
Handler::~Handler()
{
	rm_ref();
}

templalte<typename T>
Handler& Handler::operator=(const Handler &h)
{
	++h->count;
	rm_ref();

	count = h.count;
	t = h.t;
	
	return *this;
}


template<typename T>
Handler& Handler::operator*()
{
	if(t)
		return *t;
	throw std::runtime_error("dereference of unbound pointer!!!");
}  

template<typename T>
const Handler& Handler::operator*() const
{
	if(t)
	{
		return *t;
	}
	throw std::runtime_error("dereference of unbound pointer!!!");
}  


template<typename T>
Handler* Handler::operator->()
{
	if(t)	
		return t;
	throw std::runtime_error("access througn unbound Handler!!!");
}

template<typename T>
const Handler* Handler::operator->() const
{
	if(t)  
	{	
		return t;
	}
	throw std::runtime_error("access througn unbound Handler!!!");
}



