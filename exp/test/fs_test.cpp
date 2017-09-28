#include <unistd.h>
#include <iostream>

int main(){
	if( 0 == access("isx", R_OK) ){
		std::cout << "exist\n";	
	}else{
		std::cout << "exist\n";	
	}
	
	return 0;
}
