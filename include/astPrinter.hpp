#ifndef LOX_ASTPRINTER
#define LOX_ASTPRINTER

#include "expr.hpp"

class AstPrinter : public Visitor<std::string>
{
public:
    std::string print(Expr* _expr);
};

#endif