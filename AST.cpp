#include "AST.h"

bool Binary::calc() 
{
	if(op == '+')
		return left->calc() && right->calc();
	return left->calc() && right->calc();
}

bool Unary::calc()
{
	return !(e->calc());
}

bool Val::calc()
{
	return v;
}

bool Var::calc()
{
	return v;
}

void Var::set(bool v)
{
	this->v = v;
}


