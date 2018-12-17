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

Var::Var(Token* _name, Expr* _expr)
{
    name = _name;
    expr = _expr;
}

void Var::accept(Visitor<void>* visitor)
{
    return visitor->visitVarStmt(this);
}