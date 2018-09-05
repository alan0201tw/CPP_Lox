#ifndef LOX_EXPR
#define LOX_EXPR

#include "token.hpp"

#include <vector>

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

class Call : public Expr
{
private:
    Expr* callee;
    Token* paren;
    std::vector<Expr>* arguments;
public:
    Call(Expr* _callee, Token* _paren, std::vector<Expr>* _arguments);

    virtual std::string accept (Visitor<std::string>* visitor) override;
};

class Get : public Expr
{
private:
    Expr* object;
    Token* name;
public:
    Get(Expr* _object, Token* _name);

    virtual std::string accept (Visitor<std::string>* visitor) override;
};

class Grouping : public Expr
{
private:
    Expr* expression;
public:
    Grouping(Expr* _expression);

    virtual std::string accept (Visitor<std::string>* visitor) override;
};

class LiteralExpr : public Expr
{
private:
    Literal* literal;
public:
    LiteralExpr(Literal* _literal);

    virtual std::string accept (Visitor<std::string>* visitor) override;
};

class Logical : public Expr
{
private:
    Expr* left;
    Token* optr;
    Expr* right;
public:
    Logical(Expr* _left, Token* _optr, Expr* _right);

    virtual std::string accept (Visitor<std::string>* visitor) override;
};

class Set : public Expr
{
private:
    Expr* object;
    Token* name;
    Expr* value;
public:
    Set(Expr* _object, Token* _name, Expr* _value);

    virtual std::string accept (Visitor<std::string>* visitor) override;
};

class Super : public Expr
{
private:
    Token* keyword;
    Token* method;
public:
    Super(Token* _keyword, Token* _method);

    virtual std::string accept (Visitor<std::string>* visitor) override;
};

class This : public Expr
{
private:
    Token* keyword;
public:
    This(Token* _keyword);

    virtual std::string accept (Visitor<std::string>* visitor) override;
};

class Unary : public Expr
{
private:
    Token* optr;
    Expr* right;
public:
    Unary(Token* _optr, Expr* _right);

    virtual std::string accept (Visitor<std::string>* visitor) override;
};

class Variable : public Expr
{
private:
    Token* name;
public:
    Variable(Token* _name);

    virtual std::string accept (Visitor<std::string>* visitor) override;
};

// interface for visiting Exprs
template<class R>
class Visitor
{
public:
    virtual R visitAssignExpr(Assign* expr) = 0;
    virtual R visitBinaryExpr(Binary* expr) = 0;         
    virtual R visitCallExpr(Call* expr) = 0;
    virtual R visitGetExpr(Get* expr) = 0;
    virtual R visitGroupingExpr(Grouping* expr) = 0;
    virtual R visitLiteralExpr(LiteralExpr* expr) = 0;
    virtual R visitLogicalExpr(Logical* expr) = 0;
    virtual R visitSetExpr(Set* expr) = 0;
    virtual R visitSuperExpr(Super* expr) = 0;
    virtual R visitThisExpr(This* expr) = 0;
    virtual R visitUnaryExpr(Unary* expr) = 0;
    virtual R visitVariableExpr(Variable* expr) = 0;
};

#endif