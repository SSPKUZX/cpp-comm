#include <iostream>
#include "Expr.h"

int main()
{
	const std::string plus("+"), minus("-"),multiply("*");
	Expr expr(multiply,
		 Expr(minus, Expr(5)),
		 Expr(plus, Expr(3), Expr(4)));

	std::cout << expr << std::endl;

	std::cout << expr.eval();
	
	return 0;
}
