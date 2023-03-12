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
	tok_eof,
	tok_err,
};

typedef struct {
	std::string value;
	Token tok;
} Lexeme;

std::queue<Lexeme> lexer(std::string &equation);
void parser(std::queue<Lexeme> &input);
bool equation(std::queue<Lexeme> &input);
bool disjunction(std::queue<Lexeme> &input);
bool conjunction(std::queue<Lexeme> &input);
bool flip(std::queue<Lexeme> &input);
bool atom(std::queue<Lexeme> &input);
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
	std::queue<Lexeme> report = lexer(equation);
	if(report.size() == 0)
		std::cerr << "Error: No equation was given" << std::endl;
	else if(report.front().tok == tok_err)
		return -1;
	else
		parser(report);
	/*
	while(report.size() != 0)
	{
		std::cout << report.front().value << " | " << report.front().tok << std::endl;
		report.pop();
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
std::queue<Lexeme> lexer(std::string &equation)
{
	std::queue<Lexeme> report;
	if(equation.size() == 0)
		return report;

	std::string item;
	Token tok;
	int i = 0;
	while(true) {
		for(i = i; i < equation.size() && isspace(equation[i]); i++)
			;
		if(i >= equation.size())
			return report;
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
					report.front().tok = tok_err;
					return report;
			}
			i++;
		}
		Lexeme temp = {item, tok};
		report.push(temp);
	}
}

void parser(std::queue<Lexeme> &input)
{
	if(input.size() == 0 || input.front().tok == tok_err)
		return;
	if(equation(input))
		std::cout << "Valid String" << std::endl;
	else
		std::cout << "Invalid String" << std::endl;
}

bool equation(std::queue<Lexeme> &input)
{
	return disjunction(input) && (input.size() == 0);
}

bool disjunction(std::queue<Lexeme> &input)
{
	if(input.size() == 0)
		return false;
	if(conjunction(input)) {
		if(input.front().tok == tok_or) {
			input.pop();
			if(!disjunction(input)) {
				return err_message("AND_EXPR", input.front());
			}
		}
		return true;
	}
	return false;
}

bool conjunction(std::queue<Lexeme> &input)
{
	if(input.size() == 0)
		return false;
	if(flip(input)) {
		if(input.front().tok == tok_and) {
			input.pop();
			if(!flip(input)) {
				return err_message("FLIP_EXPR", input.front());
			}
		}
		return true;
	}
	return false;
}

bool flip(std::queue<Lexeme> &input)
{
	if(input.size() == 0)
		return false;
	if(input.front().tok == tok_not)
		input.pop();
	return atom(input);
}

bool atom(std::queue<Lexeme> &input)
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
			disjunction(input);
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
	return false;
}
