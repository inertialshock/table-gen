#ifndef AST_H
#define AST_H
class Expr {
	public:
		virtual ~Expr() = default;
		virtual bool calc() {};
};
class Binary : public Expr {
	public:
		Binary(char op, Expr *left, Expr *right)
			: op(op), left(left), right(right) {}
		bool calc() override;
	private:
		char op;
		Expr *left, *right;
};
class Unary : public Expr {
	public:
		Unary(Expr *e)
			: e(e) {}
		bool calc() override;
	private:
		Expr *e;
};
class Unit : public Expr {
	public:
		Unit(bool val)
			: val(val) {}
		bool calc() override;
	private:
		bool val;
};
#endif
