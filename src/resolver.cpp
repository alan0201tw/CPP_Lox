#include "resolver.hpp"

Resolver::Resolver(Interpreter* _interpreter)
    : interpreter(_interpreter) {}

void Resolver::visitBlockStmt(Block* stmt)
{
    beginScope();
    resolve(stmt->statements);
    endScope();

    return;
}

void visitVarStmt(Var* stmt)
{
    declare(stmt->name);
    if(stmt->initializer != nullptr)
    {
        resolve(stmt->initializer);
    }
    define(stmt->name);
    return;
}

void Resolver::resolve(std::vector<Stmt*> _statements)
{
    for(int i = 0; i < _statements.size(); i++)
    {
        resolve(_statements[i]);
    }
}

void Resolver::resolve(Stmt* _statement)
{
    _statement->accept(this);
}

void Resolver::resolve(Expr* _expression)
{
    _expression->accept(this);
}

void Resolver::beginScope()
{
    scopes.push(new stringToBoolMap());
}

void Resolver::endScope()
{
    scopes.pop();
}