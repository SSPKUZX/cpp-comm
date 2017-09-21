#ifndef EXPR_H_
#define EXPR_H_

#include <iostream>
#include <string>
#include "ExprNode.h"

class Expr
{
	friend std::ostream& operator<< (std::ostream&, const Expr&);

	//delegate of expression
	ExprNode *expr;

	//declaration of public interfaces
	public:
	Expr(int );
	Expr(const std::string&, Expr );
	Expr(const std::string&, Expr , Expr );

	Expr(const Expr&);
	Expr& operator= (const Expr&);
	int eval() const;
	~Expr();
};

#endif
