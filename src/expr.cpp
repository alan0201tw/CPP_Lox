#include "expr.hpp"

Assign::Assign(Token* _name, Expr* _value)
{
    name = _name;
    value = _value;
}

std::string Assign::accept(Visitor<std::string>* visitor)
{
    return visitor->visitAssignExpr(this);
}