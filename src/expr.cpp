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

Token* Assign::accept(Visitor<Token*>* visitor)
{
    return visitor->visitAssignExpr(this);
}

void Assign::accept(Visitor<void>* visitor)
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

Token* Binary::accept(Visitor<Token*>* visitor)
{
    return visitor->visitBinaryExpr(this);
}

void Binary::accept(Visitor<void>* visitor)
{
    return visitor->visitBinaryExpr(this);
}

Call::Call(Expr* _callee, Token* _closingParen, std::vector<Expr*> _arguments)
{
    callee = _callee;
    closingParen = _closingParen;
    arguments = _arguments;
}

std::string Call::accept(Visitor<std::string>* visitor)
{
    return visitor->visitCallExpr(this);
}

Token* Call::accept(Visitor<Token*>* visitor)
{
    return visitor->visitCallExpr(this);
}

void Call::accept(Visitor<void>* visitor)
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

Token* Get::accept(Visitor<Token*>* visitor)
{
    return visitor->visitGetExpr(this);
}

void Get::accept(Visitor<void>* visitor)
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

Token* Grouping::accept(Visitor<Token*>* visitor)
{
    return visitor->visitGroupingExpr(this);
}

void Grouping::accept(Visitor<void>* visitor)
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

Token* LiteralExpr::accept(Visitor<Token*>* visitor)
{
    return visitor->visitLiteralExpr(this);
}

void LiteralExpr::accept(Visitor<void>* visitor)
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

Token* Logical::accept(Visitor<Token*>* visitor)
{
    return visitor->visitLogicalExpr(this);
}

void Logical::accept(Visitor<void>* visitor)
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

Token* Set::accept(Visitor<Token*>* visitor)
{
    return visitor->visitSetExpr(this);
}

void Set::accept(Visitor<void>* visitor)
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

Token* Super::accept(Visitor<Token*>* visitor)
{
    return visitor->visitSuperExpr(this);
}

void Super::accept(Visitor<void>* visitor)
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

Token* This::accept(Visitor<Token*>* visitor)
{
    return visitor->visitThisExpr(this);
}

void This::accept(Visitor<void>* visitor)
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

Token* Unary::accept(Visitor<Token*>* visitor)
{
    return visitor->visitUnaryExpr(this);
}

void Unary::accept(Visitor<void>* visitor)
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

Token* Variable::accept(Visitor<Token*>* visitor)
{
    return visitor->visitVariableExpr(this);
}

void Variable::accept(Visitor<void>* visitor)
{
    return visitor->visitVariableExpr(this);
}