#ifndef AST_H
#define AST_H
#include <string>
class Expr {
	public:
		virtual bool calc() {return true;}
};
class Binary : public Expr {
	public:
		Binary(char op, Expr *left, Expr *right)
			: op(op), left(left), right(right) {}
		bool calc() override;
		~Binary();
	private:
		char op;
		Expr *left, *right;
};
class Unary : public Expr {
	public:
		Unary(Expr *e)
			: e(e) {}
		bool calc() override;
		~Unary();
	private:
		Expr *e;
};
class Val : public Expr {
	public:
		Val(bool v)
			: v(v) {}
		bool calc() override;
	private:
		bool v;
};

class Var : public Expr {
	public:
		Var(std::string &name)
			: name(name) {}
		bool calc() override;
		void set(bool v);
	private:
		std::string name;
		bool v;
};
#endif
