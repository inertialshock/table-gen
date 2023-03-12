#include "AST.h"

bool Binary::calc() 
{
	if(op == '+')
		return left->calc() || right->calc();
	return left->calc() && right->calc();
}

Binary::~Binary() {
	delete left;
	delete right;
}

bool Unary::calc()
{
	return !(e->calc());
}

Unary::~Unary() {
	delete e;
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
