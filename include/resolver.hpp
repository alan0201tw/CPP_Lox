#ifndef LOX_RESOLVER
#define LOX_RESOLVER

#include "expr.hpp"
#include "stmt.hpp"
#include "interpreter.hpp"

#include <vector>
#include <stack>
#include <map>

class Resolver : public Expr::Visitor<void>, Stmt::Visitor<void>
{
private:
    typedef std::map<std::string, bool> stringToBoolMap;

    Interpreter* const interpreter;
    // Keys, as in Environment, are variable names.
    std::stack<stringToBoolMap*> scopes;

public:
    Resolver(Interpreter* _interpreter);

    virtual void visitBlockStmt(Block* stmt) override;
    virtual void visitVarStmt(Var* stmt) override;

private:
    void resolve(std::vector<Stmt*> _statements);
    void resolve(Stmt* _statement);
    void resolve(Expr* _expression);

    void beginScope();
    void endScope();
};

#endif