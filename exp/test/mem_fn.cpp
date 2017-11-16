#include <utility>
#include <cassert>
#include <iostream>

template<class ClassType, class RetType, class... Args>
using MemFn = RetType(ClassType::*)(Args...);

template<class ClassType, class RetType, class... Args>
using CMemFn = RetType(ClassType::*)(Args...) const;

template<class ClassType, class RetType, class... Args>
using RMemFn = RetType(ClassType::*)(Args...) &;

template<class ClassType, class RetType, class... Args>
using CRMemFn = RetType(ClassType::*)(Args...) const&;

template<class ClassType, class RetType, class... Args>
using MMemFn = RetType(ClassType::*)(Args...) &&;

struct Test{
	int Say(int){return 0;}
};

template<class... Args>
using TestRpcMemFn = MemFn<Test, int, Args...>; 

template<class... Args>
int tell( TestRpcMemFn<Args...> memFn, Test* tptr, Args&&... args ){
	return (tptr->*memFn)(std::forward<Args>(args)...);	
}

int main(){

	Test t;	
	assert(0==tell(&Test::Say, &t, 3));

	return 0;
}


