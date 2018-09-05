#include "astPrinter.hpp"

std::string AstPrinter::print(Expr* _expr)
{
    return _expr->accept(this);
}