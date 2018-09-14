#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include "expr.hpp"

// in jlox, the return type of the visitor is a Object, which can be any type in Java.
// in cpplox, I use the Token class for this since it contains every information we need to form a object, with
// some extra fields that can be left empty. Also, tokens for this purpose should be within these types :
// NUMBER, STRING, TRUE, FALSE. We need these types to represent double, string, bool values.
class Interpreter : public Visitor<Token*>
{
public:
    Token* visitAssignExpr(Assign* expr);
    Token* visitBinaryExpr(Binary* expr);         
    Token* visitCallExpr(Call* expr);
    Token* visitGetExpr(Get* expr);
    Token* visitGroupingExpr(Grouping* expr);
    Token* visitLiteralExpr(LiteralExpr* expr);
    Token* visitLogicalExpr(Logical* expr);
    Token* visitSetExpr(Set* expr);
    Token* visitSuperExpr(Super* expr);
    Token* visitThisExpr(This* expr);
    Token* visitUnaryExpr(Unary* expr);
    Token* visitVariableExpr(Variable* expr);

private:
    Token* evaluate(Expr* expr);
    bool isTruthy(Token* _token);
};

#endif