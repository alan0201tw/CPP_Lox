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

Binary::Binary(Expr* _left, Token* _optr, Expr* _right)
{
    left = _left;
    optr = _optr;
    right = _right;
}

std::string Binary::accept(Visitor<std::string>* visitor)
{
    return visitor->visitBinaryExpr(this);
}

Call::Call(Expr* _callee, Token* _paren, std::vector<Expr>* _arguments)
{
    callee = _callee;
    paren = _paren;
    arguments = _arguments;
}

std::string Call::accept(Visitor<std::string>* visitor)
{
    return visitor->visitCallExpr(this);
}

Get::Get(Expr* _object, Token* _name)
{
    object = _object;
    name = _name;
}

std::string Get::accept(Visitor<std::string>* visitor)
{
    return visitor->visitGetExpr(this);
}

Grouping::Grouping(Expr* _expression)
{
    expression = _expression;
}

std::string Grouping::accept(Visitor<std::string>* visitor)
{
    return visitor->visitGroupingExpr(this);
}

LiteralExpr::LiteralExpr(Token* _value)
{
    value = _value;
}

std::string LiteralExpr::accept(Visitor<std::string>* visitor)
{
    return visitor->visitLiteralExpr(this);
}

Logical::Logical(Expr* _left, Token* _optr, Expr* _right)
{
    left = _left;
    optr = _optr;
    right = _right;
}

std::string Logical::accept(Visitor<std::string>* visitor)
{
    return visitor->visitLogicalExpr(this);
}

Set::Set(Expr* _object, Token* _name, Expr* _value)
{
    object = _object;
    name = _name;
    value = _value;
}

std::string Set::accept(Visitor<std::string>* visitor)
{
    return visitor->visitSetExpr(this);
}

Super::Super(Token* _keyword, Token* _method)
{
    keyword = _keyword;
    method = _method;
}

std::string Super::accept(Visitor<std::string>* visitor)
{
    return visitor->visitSuperExpr(this);
}

This::This(Token* _keyword)
{
    keyword = _keyword;
}

std::string This::accept(Visitor<std::string>* visitor)
{
    return visitor->visitThisExpr(this);
}

Unary::Unary(Token* _optr, Expr* _right)
{
    optr = _optr;
    right = _right;
}

std::string Unary::accept(Visitor<std::string>* visitor)
{
    return visitor->visitUnaryExpr(this);
}

Variable::Variable(Token* _name)
{
    name = _name;
}

std::string Variable::accept(Visitor<std::string>* visitor)
{
    return visitor->visitVariableExpr(this);
}