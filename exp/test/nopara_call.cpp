#include <cassert>

template<class FuncType, class... Args>
typename std::result_of<FuncType(Args...)>::type invoke(FuncType&& func, Args&&... args){
	return std::forward<FuncType>(func)( std::forward<Args>(args)... );
}

template<class ObjType, class RetType, class RObjType, class... Args>
RetType invoke<RetType(ObjType::*)(Args...), RObjType, Args...>(RetType(ObjType::*)(Args...) memFunc, RObjType&& robj, Args&&... args){
	return std::forward<RObjType>(robj).(*memFunc)( std::forward<Args>(args)... );
}
