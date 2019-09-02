#ifndef LOX_RESOLVER
#define LOX_RESOLVER

#include "expr.hpp"
#include "stmt.hpp"
#include "interpreter.hpp"

class Resolver : public Expr::Visitor<void>, Stmt::Visitor<void>
{
private:
    Interpreter* const interpreter;

public:
    Resolver(Interpreter* _interpreter);
};

#endif