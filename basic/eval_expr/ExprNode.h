#ifndef EXPR_NODE_H_
#define EXPR_NODE_H_

#include <iostream>

//forward declaration of Expr
class Expr;

/*
act as the virtual expression node 
*/
class ExprNode
{
	friend std::ostream& operator<< (std::ostream&, const Expr&);
	friend class Expr;

	int count;

	protected:
	ExprNode() : count(1) { }
	virtual ~ExprNode();
	virtual void print(std::ostream& ) const = 0;
	virtual int eval() const = 0;
};


#endif
