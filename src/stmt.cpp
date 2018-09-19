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