#include <iostream>
#include <string>
#include <queue>
#include <vector>

#include <ctype.h>
#include <math.h>

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
std::vector<Var *> symbol_table;

bool lexer(std::string &equation);
Expr* parser();
Expr* equation();
Expr* disjunction();
Expr* conjunction();
Expr* flip();
Expr* atom();
Expr* err_message(const char *message, Lexeme &item);
void print_table(Expr *e);
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
	else {
		Expr* e = parser();
		if(!e) {
			std::cerr << "Invalid string" << std::endl;
			return -1;
		}
		else {
			if(symbol_table.size() > 0) {
				print_table(e);
			}
			else {
				std::cout << "Value of equation: " << e->calc() << std::endl;
			}
		}
	}
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

Expr* parser()
{
	if(input.size() == 0)
		return nullptr;
	return equation();
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
	else if(input.front().tok == tok_var) {
		for(int i = 0; i < symbol_table.size(); i++) {
			if(symbol_table[i]->name == input.front().value) {
				input.pop();
				return ((Expr*) symbol_table[i]);
			}
		}
		temp = new Var(input.front().value);
		symbol_table.push_back((Var *) temp);
	}
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

void print_table(Expr *e)
{
	int longest_name = symbol_table[0]->name.length();
	for(int i = 1; i < symbol_table.size(); i++) {
		if(symbol_table[i]->name.length() > longest_name)
			longest_name = symbol_table[i]->name.length();
	}
	std::string header = "";
	for(int i = 0; i < symbol_table.size(); i++) {
		header += ("| " + symbol_table[i]->name);
		for(int j = 0; j <= longest_name - symbol_table[i]->name.length(); j++)
			header += " ";
	}
	header += "| Y";
	for(int i = 0; i <= longest_name - 1; i++)
		header += " ";
	header += "|\n";

	std::string line = "";
	for(int i = 0; i <= symbol_table.size(); i++) {
		line += "|";
		for(int j = 0; j < longest_name + 2; j++)
			line += "-";
	}
	line += "|\n";
	header += line;

	for(int i = 0; i < ((int) pow(2, symbol_table.size())); i++) {
		line = "";
		for(int j = symbol_table.size() - 1; j >= 0; j--) {
			symbol_table[symbol_table.size() - 1 - j]->set((i >> j) & 1);
			line += ("| " + std::to_string((i >> j) & 1));
			for(int k = 0; k <= longest_name - 1; k++)
				line += " ";
		}
		line += ("| " + std::to_string(e->calc()));
		for(int k = 0; k <= longest_name - 1; k++)
			line += " ";
		header += (line + "|\n");
	}
	std::cout << header << std::endl;
}
