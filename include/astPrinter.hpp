#ifndef LOX_ASTPRINTER
#define LOX_ASTPRINTER

#include "expr.hpp"

#include <vector>

class AstPrinter : public Visitor<std::string>
{
public:
    std::string print(Expr* _expr);

    virtual std::string visitAssignExpr(Assign* expr) override;
    virtual std::string visitBinaryExpr(Binary* expr) override;
    virtual std::string visitCallExpr(Call* expr) override;
    virtual std::string visitGetExpr(Get* expr) override;
    virtual std::string visitGroupingExpr(Grouping* expr) override;
    virtual std::string visitLiteralExpr(LiteralExpr* expr) override;
    virtual std::string visitLogicalExpr(Logical* expr) override;
    virtual std::string visitSetExpr(Set* expr) override;
    virtual std::string visitSuperExpr(Super* expr) override;
    virtual std::string visitThisExpr(This* expr) override;
    virtual std::string visitUnaryExpr(Unary* expr) override;
    virtual std::string visitVariableExpr(Variable* expr) override;

private:
    // utility methods
    std::string parenthesize(std::string _name, std::vector<Expr*> _exprs);
};

#endif