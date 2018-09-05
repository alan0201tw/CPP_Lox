#ifndef LOX_EXPR
#define LOX_EXPR

#include "token.hpp"

template<class R> class Visitor;

// interface for Expr classes
class Expr
{
public:
    // set function to 0 to make it an abstract function
    virtual std::string accept(Visitor<std::string>* visitor) = 0;

    // I use this workaround since the tutorial uses virtual generic methods,
    // which isn't possible in C++. After doing some research, I come up with this
    // not-so-good approach. Please don't judge me for this Q_Q
};

class Assign : public Expr
{
private:
    Token* name;
    Expr* value;

public:
    Assign(Token* _name, Expr* _value);

    virtual std::string accept (Visitor<std::string>* visitor) override;
};

class Binary : public Expr
{
private:
    Expr* left;
    Token* optr;
    Expr* right;
public:
    Binary(Expr* _left, Token* _optr, Expr* _right);

    virtual std::string accept (Visitor<std::string>* visitor) override;
};


// interface for visiting Exprs
template<class R>
class Visitor
{
public:
    virtual R visitAssignExpr(Assign* expr) = 0;

    /* uncomment when classes are implemented

    virtual R visitBinaryExpr(Binary* expr) = 0;         
    virtual R visitCallExpr(Call* expr) = 0;
    virtual R visitGetExpr(Get* expr) = 0;
    virtual R visitGroupingExpr(Grouping* expr) = 0;
    virtual R visitLiteralExpr(Literal* expr) = 0;
    virtual R visitLogicalExpr(Logical* expr) = 0;
    virtual R visitSetExpr(Set* expr) = 0;
    virtual R visitSuperExpr(Super* expr) = 0;
    virtual R visitThisExpr(This* expr) = 0;
    virtual R visitUnaryExpr(Unary* expr) = 0;
    virtual R visitVariableExpr(Variable* expr) = 0;

    */
};

#endif