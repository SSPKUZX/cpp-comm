//http://www.cppblog.com/misakamm/archive/2012/01/16/164258.html
//#include "to.h"

#include <cassert>
#include <iostream>

#define comac_get_args_cnt( ... ) comac_arg_n( __VA_ARGS__ )
#define comac_arg_n( _0,_1,_2,_3,_4,_5,_6,_7,N,...) N
#define comac_args_seqs() 7,6,5,4,3,2,1,0
#define comac_join_1( x,y ) x##y

#define comac_argc( ... ) comac_get_args_cnt( 0,##__VA_ARGS__,comac_args_seqs() )
#define comac_join( x,y) comac_join_1( x,y )

//-- 1.2 repeat
#define repeat_0( fun,a,... )  
#define repeat_1( fun,a,... ) fun( 1,a,__VA_ARGS__ ) repeat_0( fun,__VA_ARGS__ )
#define repeat_2( fun,a,... ) fun( 2,a,__VA_ARGS__ ) repeat_1( fun,__VA_ARGS__ )
#define repeat_3( fun,a,... ) fun( 3,a,__VA_ARGS__ ) repeat_2( fun,__VA_ARGS__ )
#define repeat_4( fun,a,... ) fun( 4,a,__VA_ARGS__ ) repeat_3( fun,__VA_ARGS__ )
#define repeat_5( fun,a,... ) fun( 5,a,__VA_ARGS__ ) repeat_4( fun,__VA_ARGS__ )
#define repeat_6( fun,a,... ) fun( 6,a,__VA_ARGS__ ) repeat_5( fun,__VA_ARGS__ )

#define repeat( n,fun,... ) comac_join( repeat_,n )( fun,__VA_ARGS__)


#define link_attr( i,attr,... )	t##attr, 
#define stringize( i,a,... )	#a,

#define concat(...) repeat( comac_argc(__VA_ARGS__) ,link_attr,__VA_ARGS__ ) ... 
#define strings(...) repeat( comac_argc(__VA_ARGS__) ,stringize,__VA_ARGS__ ) ...   

int main(){
//	assert("tatbtc"==to(strs(a,b,c)));
/*	for(auto& str : { strings(a,b,c)}){
		std::cout << str << '\t';	
	}
	std::cout << '\n';
*/		
	// the last dota should be dealt
	std::string a = strings(3) b="";
	std::cout << a << '\n';
//	std::cout << b << '\n';
	return 0;
}
