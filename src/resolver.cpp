#include "resolver.hpp"

#include "token.hpp"
#include "lox.hpp"

Resolver::Resolver(Interpreter* _interpreter)
    : interpreter(_interpreter) {}

void Resolver::visitBlockStmt(Block* stmt)
{
    beginScope();
    resolve(stmt->statements);
    endScope();

    return;
}

void Resolver::visitVarStmt(Var* stmt)
{
    declare(stmt->name);
    if(stmt->initializer != nullptr)
    {
        resolve(stmt->initializer);
    }
    define(stmt->name);
    return;
}

void Resolver::visitVariableExpr(Variable* expr)
{
    if(scopes.empty() == false && scopes.back()->at(expr->name->lexeme) == false)
    {
        Lox::error(expr->name, "Cannot read local variable in its own initializer.");
    }

    resolveLocal(expr, expr->name);
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

void Resolver::resolveLocal(Expr* _expr, Token* _name)
{
    for (auto const& element : scopes)
    {
        // if this map (element) contains the key, _name->lexeme
        if(element->find(_name->lexeme) != element->end())
        {
            interpreter.resolve(_expr, scopes.size() - 1 - i);
            return;
        }
    }

    // Not found. Assume it is global.
}

void Resolver::declare(Token* _name)
{
    if(scopes.empty() == true)
    {
        return;
    }
    auto scope = scopes.back();
    scope->insert_or_assign(_name->lexeme, false);
}

void Resolver::define(Token* _name)
{
    if(scopes.empty() == true)
    {
        return;
    }
    scopes.back()->insert_or_assign(_name->lexeme, true);
}

void Resolver::beginScope()
{
    //scopes.push(new stringToBoolMap());
    scopes.push_back(new stringToBoolMap());
}

void Resolver::endScope()
{
    //scopes.pop();
    scopes.pop_back();
}