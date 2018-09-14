#include "astPrinter.hpp"

#include <stdexcept>

std::string AstPrinter::print(Expr* _expr)
{
    return _expr->accept(this);
}

std::string AstPrinter::visitAssignExpr(Assign* expr)
{
    throw std::logic_error("Function not yet implemented.");
}

std::string AstPrinter::visitBinaryExpr(Binary* expr)
{
    // use {a,b,...} to create anonymous vectors
    return parenthesize(expr->optr->lexeme, {expr->left, expr->right});
}

std::string AstPrinter::visitCallExpr(Call* expr)
{
    throw std::logic_error("Function not yet implemented.");
}

std::string AstPrinter::visitGetExpr(Get* expr)
{
    throw std::logic_error("Function not yet implemented.");
}

std::string AstPrinter::visitGroupingExpr(Grouping* expr)
{
    return parenthesize("group", {expr->expression});
}

std::string AstPrinter::visitLiteralExpr(LiteralExpr* expr)
{
    // null check
    if(expr->value->literal)
    {
        return expr->value->lexeme;
        // return expr->value->toString();
    }
    return "nil";
}

std::string AstPrinter::visitLogicalExpr(Logical* expr)
{
    throw std::logic_error("Function not yet implemented.");
}

std::string AstPrinter::visitSetExpr(Set* expr)
{
    throw std::logic_error("Function not yet implemented.");
}

std::string AstPrinter::visitSuperExpr(Super* expr)
{
    throw std::logic_error("Function not yet implemented.");
}

std::string AstPrinter::visitThisExpr(This* expr)
{
    throw std::logic_error("Function not yet implemented.");
}

std::string AstPrinter::visitUnaryExpr(Unary* expr)
{
    return parenthesize(expr->optr->lexeme, {expr->right});
}

std::string AstPrinter::visitVariableExpr(Variable* expr)
{
    throw std::logic_error("Function not yet implemented.");
}

std::string AstPrinter::parenthesize(std::string _name, std::vector<Expr*> _exprs)
{
    std::string str = "";

    str.append("(").append(_name);
    for(size_t i = 0; i < _exprs.size(); i++)
    {
        str.append(" ");
        str.append(_exprs[i]->accept(this));
    }
    str.append(")");

    return str;
}