#ifndef CONSTANT_NODE_H_
#define CONSTANT_NODE_H_

#include <iostream>
#include "ExprNode.h"
#include "Expr.h"

class ConstantNode : public ExprNode
{
	friend class Expr;
	
	int value;

	ConstantNode(int val) : value(val){}

	void print(std::ostream& out) const
	{
		out << value;
	}

	int eval() const { return value; }
};

#endif
