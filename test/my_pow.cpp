//https://leetcode.com/problems/powx-n/#/description

#include <iostream>
#include <stack>
#include <limits>

double myPow(double x, int n) {
	if(x==0) return 0;

	bool negative = n<0;        
	// minimal integer can't convert to a positive integer
	bool is_minimal=false;
	if(n==std::numeric_limits<int>::min()) {
		is_minimal=true;
		++n;
	}
	if(negative) n*=-1;

	double r=1.0;
	std::stack<bool>	stk;	
	while(n>0){
		stk.push(n%2==1);
		n/=2;
	}		
	while(!stk.empty()){
		r*=r;
		if(stk.top()) r*=x;	
		stk.pop();
	}
	if(is_minimal) r*=x;
	return negative?1/r:r;
}

template<int x, int y>
struct pow : std::integral_constant<long,  pow<x,y/2>::value * pow<x,y/2>::value * (y%2==0? 1 : x) > {};
template<int x>
struct pow<x, 0> : std::integral_constant<long, 1 > {};
template<int x>
struct pow<x, 1> : std::integral_constant<long, x > {};
template<int y>
struct pow<0, y>{ static_assert(false, "x can't be zero in pow"); };
template<>
struct pow<0, 0>{ static_assert(false, "x can't be zero in pow"); };
template<>
struct pow<0, 1>{ static_assert(false, "x can't be zero in pow"); };

int main()
{
	std::cout << pow<2,3>::value << '\n';
	std::cout << pow<2,0>::value << '\n';
	std::cout << pow<2,1>::value << '\n';
	std::cout << pow<0,4>::value << '\n';
	std::cout << pow<0,1>::value << '\n';
	std::cout << pow<0,0>::value << '\n';

	std::cout << "{3,3}\t" << myPow(3,3) << '\n';	
	std::cout << "{3,-3}\t" <<myPow(3,-3) << '\n';	
	std::cout << "{-3,3}\t" <<myPow(-3,3) << '\n';	
	std::cout << "{-3,-3}\t" <<myPow(-3,-3) << '\n';	
	std::cout << "{3,0}\t" <<myPow(3,0) << '\n';	
	std::cout << "{0,3}\t" <<myPow(0,3) << '\n';	
	std::cout << "{-3,-1}\t" <<myPow(-3,-1) << '\n';	
	std::cout << "{3,1}\t" <<myPow(3,1) << '\n';	
	// expected 0
	std::cout << "{2,-2147483648}" <<myPow(2,-2147483648) << '\n';	
	std::cout << "{2,2147483647}" <<myPow(2,2147483647) << '\n';	

	return 0;
}

