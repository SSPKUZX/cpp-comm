//http://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/types/aligned_storage.html
//http://zh.cppreference.com/w/cpp/types/aligned_storage
#include <cassert>
#include <type_traits>

#include <string>
#include <iostream>

template<class T>
class AlignedStorage{
	public:
		template<class... Args>
		AlignedStorage(Args&&... args) {
			new (&m_storage) T (std::forward<Args>(args)... );	
		}
	
		operator T&(){
		//	return *static_cast<T*>(static_cast<void*>(&m_storage));	
			return *reinterpret_cast<T*>(&m_storage);	
		}

		operator const T& () const{
		//	return *static_cast<const T*>(static_cast<const void*>(&m_storage));	
			return *reinterpret_cast<const T*>(&m_storage);	
		}
	
		~AlignedStorage(){  (this->operator T const&()).~T(); }

	private:
		typename std::aligned_storage<sizeof(T),alignof(T)>::type	m_storage;
};

template<class T>
std::ostream& operator << ( std::ostream& out, AlignedStorage<T> const& val){
	return out << static_cast<T const&>(val);
}

int main(){
	AlignedStorage<std::string>	str("hahah");
	std::cout << static_cast<std::string const&>(str) << '\n';
	std::cout << str << '\n';

	return 0;
}
