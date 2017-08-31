// 1> test if class in anonymous ns is invisible
// 2> test if can default template parameter be forwarded 

#include <iostream>

namespace utl{

	namespace{ struct Anonymous{}; }

	template<class T>
	class Pro{
		public:	
			template<class T2=T >
			void Test(T2&&){
				std::cout << "is_lvalue_reference:" << std::is_lvalue_reference<T2>::value << '\n';	
				std::cout << "is_rvalue_reference:" << std::is_rvalue_reference<T2>::value << '\n';	
			}	
	};
}

int main(){
	// case 1: visible
	utl::Anonymous anony;	

	// T2&& can be forwarded 
	utl::Pro<int> pro;
	int i=3;
	pro.Test(i);

	return 0;
}
