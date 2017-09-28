template<class Iter, class T>
auto construct(Iter p, const T& val)-> typename std::enable_if<std::is_pod<T>::value>::type
{
	*p = val;
}
template<class Iter, class T>
auto construct(Iter p, const T& val)-> typename std::enable_if<!std::is_pod<T>::value>::type
{
	new (p) T(val);
}

template<class T>
auto destroy(T* p)-> typename std::enable_if<std::is_trivally_destructible<T>::value>::type
{}

template<class T>
auto destroy(T* p)-> typename std::enable_if<!std::is_trivally_destructible<T>::value>::type
{
	p->~T();
}

//trivally_copy_construtible --->memcpy
