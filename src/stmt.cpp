#include "stmt.hpp"

Expression::Expression(Expr* _expr)
{
    expr = _expr;
}

void Expression::accept(Visitor<void>* visitor)
{
    return visitor->visitExpressionStmt(this);
}

Print::Print(Expr* _expr)
{
    expr = _expr;
}

void Print::accept(Visitor<void>* visitor)
{
    return visitor->visitPrintStmt(this);
}

Var::Var(Token* _name, Expr* _initializer)
{
    name = _name;
    initializer = _initializer;
}

void Var::accept(Visitor<void>* visitor)
{
    return visitor->visitVarStmt(this);
}

Block::Block(std::vector<Stmt*> _statements)
{
    statements = _statements;
}

void Block::accept(Visitor<void>* visitor)
{
    return visitor->visitBlockStmt(this);
}

If::If(Expr* _condition, Stmt* _thenBranch, Stmt* _elseBranch)
{
    condition = _condition;
    thenBranch = _thenBranch;
    elseBranch = _elseBranch;
}

void If::accept(Visitor<void>* visitor)
{
    return visitor->visitIfStmt(this);
}