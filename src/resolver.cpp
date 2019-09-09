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

void Resolver::visitFunctionStmt(Function* stmt)
{
    declare(stmt->name);
    define(stmt->name);

    resolveFunction(stmt, FunctionType::FUNCTION);
    return;
}

void Resolver::visitExpressionStmt(Expression* stmt)
{
    resolve(stmt->expr);
    return;
}

void Resolver::visitIfStmt(If* stmt)
{
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if(stmt->elseBranch != nullptr)
    {
        resolve(stmt->elseBranch);
    }
    return;
}

void Resolver::visitPrintStmt(Print* stmt)
{
    resolve(stmt->expr);
    return;
}

void Resolver::visitReturnStmt(Return* stmt)
{
    if(currentFunction == FunctionType::NONE)
    {
        Lox::error(stmt->keyword, "Cannot return from top-level code.");
    }

    if(stmt->value != nullptr)
    {
        if(currentFunction == FunctionType::INITIALIZER)
        {
            Lox::error(stmt->keyword, "Cannot return a value from an initializer.");
        }
        resolve(stmt->value);
    }
    return;
}

void Resolver::visitWhileStmt(While* stmt)
{
    resolve(stmt->condition);
    resolve(stmt->body);
    return;
}

void Resolver::visitBreakStmt(Break* stmt)
{
    return;
}

void Resolver::visitClassStmt(Class* stmt)
{
    ClassType enclosingClass = currentClass;
    currentClass = ClassType::CLASS;

    declare(stmt->name);
    define(stmt->name);

    // detect inheritance chains
    if(stmt->superclass != nullptr
       && stmt->name->lexeme == stmt->superclass->name->lexeme)
    {
        Lox::error(stmt->superclass->name, "A class cannot inherit from itself.");
    }

    if(stmt->superclass != nullptr)
    {
        resolve(stmt->superclass);
    }

    // defining "this"
    beginScope();
    (*scopes.back())["this"] = true;

    for(Function* method : stmt->methods)
    {
        FunctionType declaration = FunctionType::METHOD;
        // determine if this method is "init"
        if(method->name->lexeme == "init")
        {
            declaration = FunctionType::INITIALIZER;
        }

        resolveFunction(method, declaration);
    }

    endScope();

    currentClass = enclosingClass;
    return;
}

void Resolver::visitVariableExpr(Variable* expr)
{
    // scope is not empty, and the value found is false

    if(scopes.empty() == false && 
       scopes.back()->find(expr->name->lexeme) != scopes.back()->end() &&
       scopes.back()->at(expr->name->lexeme) == false)
    {
        Lox::error(expr->name, "Cannot read local variable in its own initializer.");
    }

    resolveLocal(expr, expr->name);
    return;
}

void Resolver::visitAssignExpr(Assign* expr)
{
    resolve(expr->value);
    resolveLocal(expr, expr->name);
    return;
}

void Resolver::visitBinaryExpr(Binary* expr)
{
    resolve(expr->left);
    resolve(expr->right);
    return;
}

void Resolver::visitCallExpr(Call* expr)
{
    resolve(expr->callee);

    for(auto const& argument : expr->arguments)
    {
        resolve(argument);
    }
    return;
}

void Resolver::visitGroupingExpr(Grouping* expr)
{
    resolve(expr->expression);
    return;
}

void Resolver::visitLiteralExpr(LiteralExpr* expr)
{
    return;
}

void Resolver::visitLogicalExpr(Logical* expr)
{
    resolve(expr->left);
    resolve(expr->right);
    return;
}

void Resolver::visitUnaryExpr(Unary* expr)
{
    resolve(expr->right);
    return;
}

void Resolver::visitGetExpr(Get* expr)
{
    resolve(expr->object);
    return;
}

void Resolver::visitSetExpr(Set* expr)
{
    resolve(expr->value);
    resolve(expr->object);
    return;
}

void Resolver::visitThisExpr(This* expr)
{
    if(currentClass == ClassType::NONE)
    {
        Lox::error(expr->keyword, "Cannot use 'this' outside of a class.");
        return;
    }

    resolveLocal(expr, expr->keyword);
    return;
}

void Resolver::resolve(std::vector<Stmt*> _statements)
{
    for(size_t i = 0; i < _statements.size(); i++)
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
    // Reference : 
    // https://stackoverflow.com/questions/22269435/how-to-iterate-through-a-list-of-objects-in-c

    //std::list<stringToBoolMap*>::iterator it;
    for (int i = scopes.size() - 1; i >= 0; i--)
    {
        // if this map (element) contains the key, _name->lexeme
        if(scopes[i]->find(_name->lexeme) != scopes[i]->end())
        {
            interpreter->resolve(_expr, scopes.size() - 1 - i);
            return;
        }
    }

    // Not found. Assume it is global.
}

void Resolver::resolveFunction(Function* _function, FunctionType type)
{
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for(auto const& param : _function->params)
    {
        declare(param);
        define(param);
    }
    resolve(_function->body);
    endScope();

    currentFunction = enclosingFunction;
}

void Resolver::declare(Token* _name)
{
    if(scopes.empty() == true)
    {
        //std::cout << "No scopes when declaring " << _name->lexeme << std::endl;
        return;
    }
    auto scope = scopes.back();
    if(scope->find(_name->lexeme) != scope->end())
    {
        Lox::error(_name, "Variable with this name already declared in this scope.");
    }
    (*scope)[_name->lexeme] = false;
    //scope->insert_or_assign(_name->lexeme, false);
}

void Resolver::define(Token* _name)
{
    if(scopes.empty() == true)
    {
        //std::cout << "No scopes when defining " << _name->lexeme << std::endl;
        return;
    }
    // !! it should be assigning true here
    (*scopes.back())[_name->lexeme] = true;
    //scopes.back()->insert_or_assign(_name->lexeme, true);
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