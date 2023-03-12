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

bool Unit::calc()
{
	return val;
}
