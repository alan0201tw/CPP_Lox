#ifndef LOX_RESOLVER
#define LOX_RESOLVER

#include "expr.hpp"
#include "stmt.hpp"
#include "interpreter.hpp"

#include <vector>
#include <list>
#include <map>

class Resolver : public Expr::Visitor<void>, Stmt::Visitor<void>
{
private:
    typedef std::map<std::string, bool> stringToBoolMap;

    Interpreter* const interpreter;
    // Keys, as in Environment, are variable names.
    // here we use doubly-linked-list instead of stacks in the text book, since it uses the "get" method in Java stack.
    std::list<stringToBoolMap*> scopes;

public:
    Resolver(Interpreter* _interpreter);

    virtual void visitBlockStmt(Block* stmt) override;
    virtual void visitVarStmt(Var* stmt) override;

    virtual void visitVariableExpr(Variable* expr) override;

private:
    void resolve(std::vector<Stmt*> _statements);
    void resolve(Stmt* _statement);
    void resolve(Expr* _expression);

    void resolveLocal(Expr* _expr, Token* _name);

    void declare(Token* _name);
    void define(Token* _name);

    void beginScope();
    void endScope();
};

#endif