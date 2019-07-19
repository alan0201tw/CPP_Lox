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

While::While(Expr* _condition, Stmt* _body)
{
    condition = _condition;
    body = _body;
}

void While::accept(Visitor<void>* visitor)
{
    return visitor->visitWhileStmt(this);
}

Function::Function(Token* _name, std::vector<Token*> _params, std::vector<Stmt*> _body)
{
    name = _name;
    params = _params;
    body = _body;
}

void Function::accept(Visitor<void>* visitor)
{
    return visitor->visitFunctionStmt(this);
}

Return::Return(Token* _keyword, Expr* _value)
{
    keyword = _keyword;
    value = _value;
}

void Return::accept(Visitor<void>* visitor)
{
    return visitor->visitReturnStmt(this);
}