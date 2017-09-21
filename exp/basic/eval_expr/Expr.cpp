#include "ConstantNode.h"
#include "UnaryNode.h"
#include "BinaryNode.h" 

//this must be the last declaration, or the g++ would error "calling fdopen: Bad file descriptor"
#include "Expr.h"   


Expr::Expr(int val)
{
	expr = new ConstantNode(val);
}

Expr::Expr(const std::string& op, Expr  opd)
{
	expr = new UnaryNode(op, opd);
}

Expr::Expr(const std::string& op, Expr  left, Expr  right)
{
	expr = new BinaryNode(op, left, right);
}


Expr::Expr(const Expr& ep)
{
	expr = ep.expr;
	++expr->count;
}

Expr& Expr::operator= (const Expr& rhs)
{
	++rhs.expr->count;
	if(--expr->count == 0)
		delete expr;
	expr = rhs.expr;
			
	return *this;
}
	
Expr::~Expr()
{
	if(--expr->count == 0)
		delete expr;
}

int Expr::eval() const
{
	 return expr->eval();
}


  
std::ostream& operator<< (std::ostream& out, const Expr& other)
{
	other.expr->print(out);
	return out;
}



