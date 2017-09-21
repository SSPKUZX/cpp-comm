#ifndef HANDLER_H_
#define HANDLER_H_

#include <iostream>

template<typename T>
class Handler
{
	public:
	Handler(T *p = 0);
	Handler (const Handler&);
	~Handler();

	T& operator*();
	const T& operator*();
	T* operator->();
	const T* operator->();
	Handler& operator=(const Handler&);
	

	private:
	int *count;
	T *t;
	void rm_ref();
};

#endif
