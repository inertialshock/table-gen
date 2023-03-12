#include <iostream>
#include <string>
#include <queue>
#include <vector>

#include <ctype.h>

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
bool equation();
bool disjunction();
bool conjunction();
bool flip();
bool atom();
bool err_message(const char *message, Lexeme &item);
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
	if(equation())
		std::cout << "Valid String" << std::endl;
	else
		std::cout << "Invalid String" << std::endl;
}

bool equation()
{
	return disjunction() && (input.size() == 0);
}

bool disjunction()
{
	if(input.size() == 0)
		return false;
	if(conjunction()) {
		if(input.front().tok == tok_or) {
			input.pop();
			if(!disjunction()) {
				return err_message("AND_EXPR", input.front());
			}
		}
		return true;
	}
	return false;
}

bool conjunction()
{
	if(input.size() == 0)
		return false;
	if(flip()) {
		if(input.front().tok == tok_and) {
			input.pop();
			if(!flip()) {
				return err_message("FLIP_EXPR", input.front());
			}
		}
		return true;
	}
	return false;
}

bool flip()
{
	if(input.size() == 0)
		return false;
	if(input.front().tok == tok_not)
		input.pop();
	return atom();
}

bool atom()
{
	if(input.size() == 0)
		return false;
	switch(input.front().tok) {
		case tok_val:
		case tok_var:
			input.pop();
			return true;
		case tok_lparen:
			input.pop();
			disjunction();
			if(input.front().tok == tok_rparen) {
				input.pop();
				return true;
			}
			else
				return err_message(")", input.front());
		default:
			return err_message("variable or truth value", input.front());
	}
}


bool err_message(const char *message, Lexeme &item)
{
	std::cout << "Error: Expected " << message << " got " << item.value << std::endl;
	return nullptr;
}
