#ifndef BINARY_NODE_H_
#define BINARY_NODE_H_

#include <iostream>
#include <string>
#include <stdexcept>
#include "ExprNode.h"
#include "Expr.h"

class BinaryNode : public ExprNode
{
	friend class Expr;

	std::string opr;
	Expr   	left;
	Expr   	right;

	BinaryNode(const std::string& op, Expr  l, Expr  r) : opr(op), left(l), right(r){}

	void print(std::ostream& out) const
	{
		out << "(" << left << opr << right << ")"; 
	}

	int eval() const
	{
		if (opr == "+")
			return  left.eval() +  right.eval();
		if (opr == "-")
			return  left.eval() -  right.eval();
		if (opr == "*")
			return  left.eval() *  right.eval();
		if (opr == "/" &&  right.eval() != 0)
			return  left.eval() /  right.eval();
		
		throw std::runtime_error("wrong operator: " + opr + "in binary node");
	}
};

#endif
