#include <iostream>
#include <string>
#include <queue>
#include <vector>

#include <ctype.h>

#include "AST.h"

enum Token {
	tok_val,
	tok_var,
	tok_and,
	tok_or,
	tok_not,
	tok_lparen,
	tok_rparen,
};

typedef struct {
	std::string value;
	Token tok;
} Lexeme;

std::queue<Lexeme> input;
bool lexer(std::string &equation);
void parser();
Expr* equation();
Expr* disjunction();
Expr* conjunction();
Expr* flip();
Expr* atom();
Expr* err_message(const char *message, Lexeme &item);
//void print_table(AST* head);
int main(int argc, char **argv)
{
	std::string equation;
	if(argc == 1) {
		std::cout << "Input equation: ";
		std::getline(std::cin, equation);
	}
	else
		equation = argv[1];
	/*
	AST* head = parser(lexer(equation));
	if(!head->have_children())
		return -1;
	print_table(head);
	*/
	if(!lexer(equation)) {
		if(input.size() == 0)
			std::cerr << "Error: No equation was given" << std::endl;
		return -1;
	}
	else
		parser();
	/*
	while(input.size() != 0)
	{
		std::cout << input.front().value << " | " << input.front().tok << std::endl;
		input.pop();
	}
	*/
	return 0;
}

/*
 * Valid Tokens
 * * (AND) | + (OR) | ! (NOT)
 * Parentheses
 * 1/0 for truth values
 * [a-zA-Z]+ for variables
 * NOTE: Whitespace is ignored
 */
bool lexer(std::string &equation)
{
	if(equation.size() == 0) {
		std::cerr << "Error: No input given" << std::endl;
		return false;
	}

	std::string item;
	Token tok;
	int i = 0;
	while(true) {
		for(i = i; i < equation.size() && isspace(equation[i]); i++)
			;
		if(i >= equation.size())
			return true;
		item = equation[i];
		if(isalpha(equation[i])) {
			for(i = i + 1; i < equation.size() && isalnum(equation[i]); i++)
				item += equation[i];
			tok = tok_var;
		}
		else {
			switch(equation[i]) {
				case '1':
				case '0':
					tok = tok_val;
					break;
				case '+':
					tok = tok_or;
					break;
				case '*':
					tok = tok_and;
					break;
				case '!':
					tok = tok_not;
					break;
				case '(':
					tok = tok_lparen;
					break;
				case ')':
					tok = tok_rparen;
					break;
				default:
					std::cerr << "Error: Invalid token " << item << " detected at index: " << i << std::endl;
					return false;
			}
			i++;
		}
		Lexeme temp = {item, tok};
		input.push(temp);
	}
}

void parser()
{
	if(input.size() == 0)
		return;
	Expr *eq = equation();
	if(eq)
		std::cout << eq->calc() << std::endl;
	else
		std::cout << "Invalid String" << std::endl;
}

Expr* equation()
{
	Expr *eq = disjunction();
	if(eq && (input.size() == 0))
		return eq;
	return nullptr;
}

Expr* disjunction()
{
	if(input.size() == 0)
		return nullptr;
	Expr *lhs = conjunction();
	if(lhs) {
		if(input.front().tok == tok_or) {
			input.pop();
			Expr *rhs = disjunction();
			if(!rhs) {
				delete lhs;
				return err_message("OR_EXPR", input.front());
			}
			return new Binary('+', lhs, rhs);
		}
	}
	return lhs;
}

Expr* conjunction()
{
	if(input.size() == 0)
		return nullptr;
	Expr *lhs = flip();
	if(lhs) {
		if(input.front().tok == tok_and) {
			input.pop();
			Expr *rhs = conjunction();
			if(!rhs) {
				delete lhs;
				return err_message("AND_EXPR", input.front());
			}
			return new Binary('*', lhs, rhs);
		}
	}
	return lhs;
}

Expr* flip()
{
	if(input.size() == 0)
		return nullptr;
	Expr *temp;
	if(input.front().tok == tok_not) {
		input.pop();
		temp = atom();
		if(!temp)
			return nullptr;
		return new Unary(temp);
	}
	return atom();
}

Expr* atom()
{
	if(input.size() == 0)
		return nullptr;
	Expr *temp;
	if(input.front().tok == tok_val) {
		if(input.front().value == "1")
			temp = new Val(true);
		else
			temp = new Val(false);
	}
	else if(input.front().tok == tok_lparen) {
		input.pop();
		temp = disjunction();
		if(input.front().tok != tok_rparen) {
			delete temp;
			return err_message(")", input.front());
		}
	}
	else if(input.front().tok == tok_var)
		temp = new Var(input.front().value);
	else
		return err_message("variable or truth value", input.front());

	input.pop();
	return temp;
}


Expr* err_message(const char *message, Lexeme &item)
{
	std::cout << "Error: Expected " << message << " got " << item.value << std::endl;
	return nullptr;
}
