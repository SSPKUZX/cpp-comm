#ifndef UNARY_NODE_H_
#define UNARY_NODE_H_

#include <iostream>
#include <string>
#include <stdexcept>
#include "ExprNode.h"
#include "Expr.h"

class UnaryNode : public ExprNode
{
	friend class Expr;

	std::string opr;
	Expr   operand;

	UnaryNode(const std::string& op, Expr  opd) : opr(op), operand(opd){}
	
	void print(std::ostream& out) const
	{
		out << "(" << opr << operand << ")";
	}
	
	int eval() const
	{
		if (opr == "-")
			return -operand.eval();
		throw std::runtime_error("wrong operator: " + opr);
	}
};

#endif
