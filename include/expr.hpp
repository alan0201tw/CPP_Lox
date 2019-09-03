#ifndef LOX_EXPR
#define LOX_EXPR

#include <iostream>
#include <vector>

class Token;

// interface for Expr classes
class Expr
{
public:
    // Forward declaration of the Visitor interface
    template<class R> class Visitor;

    // set function to 0 to make it an abstract function
    virtual std::string accept(Visitor<std::string>* visitor) = 0; // this is for AstPrinter
    virtual Token* accept(Visitor<Token*>* visitor) = 0; // for interpreter

    virtual void accept(Visitor<void>* visitor) = 0;

    // I use this workaround since the tutorial uses virtual generic methods,
    // which isn't possible in C++. After doing some research, I come up with this
    // not-so-good approach. Please don't judge me for this. Q_Q
};

class Assign : public Expr
{
public:
    Token* name;
    Expr* value;

public:
    Assign(Token* _name, Expr* _value);

    virtual std::string accept(Visitor<std::string>* visitor) override;
    virtual Token* accept(Visitor<Token*>* visitor) override;
    virtual void accept(Visitor<void>* visitor) override;
};

class Binary : public Expr
{
public:
    Expr* left;
    Token* optr;
    Expr* right;

public:
    Binary(Expr* _left, Token* _optr, Expr* _right);

    virtual std::string accept(Visitor<std::string>* visitor) override;
    virtual Token* accept(Visitor<Token*>* visitor) override;
    virtual void accept(Visitor<void>* visitor) override;
};

class Call : public Expr
{
public:
    Expr* callee;
    Token* closingParen;
    std::vector<Expr*> arguments;

public:
    Call(Expr* _callee, Token* _closingParen, std::vector<Expr*> _arguments);

    virtual std::string accept(Visitor<std::string>* visitor) override;
    virtual Token* accept(Visitor<Token*>* visitor) override;
    virtual void accept(Visitor<void>* visitor) override;
};

class Get : public Expr
{
public:
    Expr* object;
    Token* name;

public:
    Get(Expr* _object, Token* _name);

    virtual std::string accept(Visitor<std::string>* visitor) override;
    virtual Token* accept(Visitor<Token*>* visitor) override;
    virtual void accept(Visitor<void>* visitor) override;
};

class Grouping : public Expr
{
public:
    Expr* expression;

public:
    Grouping(Expr* _expression);

    virtual std::string accept(Visitor<std::string>* visitor) override;
    virtual Token* accept(Visitor<Token*>* visitor) override;
    virtual void accept(Visitor<void>* visitor) override;
};

class LiteralExpr : public Expr
{
public:

    // in jlox, literal is stored using superclass-Object, and the toString method lives as an virtual function,
    // in out cpplox, we need an extra tag to determine the type of a literal object, we might as well just store
    // the entire Token object here.

    // Literal* literal;
    Token* value;

public:
    LiteralExpr(Token* _value);

    virtual std::string accept(Visitor<std::string>* visitor) override;
    virtual Token* accept(Visitor<Token*>* visitor) override;
    virtual void accept(Visitor<void>* visitor) override;
};

class Logical : public Expr
{
public:
    Expr* left;
    Token* optr;
    Expr* right;

public:
    Logical(Expr* _left, Token* _optr, Expr* _right);

    virtual std::string accept(Visitor<std::string>* visitor) override;
    virtual Token* accept(Visitor<Token*>* visitor) override;
    virtual void accept(Visitor<void>* visitor) override;
};

class Set : public Expr
{
public:
    Expr* object;
    Token* name;
    Expr* value;

public:
    Set(Expr* _object, Token* _name, Expr* _value);

    virtual std::string accept(Visitor<std::string>* visitor) override;
    virtual Token* accept(Visitor<Token*>* visitor) override;
    virtual void accept(Visitor<void>* visitor) override;
};

class Super : public Expr
{
public:
    Token* keyword;
    Token* method;

public:
    Super(Token* _keyword, Token* _method);

    virtual std::string accept(Visitor<std::string>* visitor) override;
    virtual Token* accept(Visitor<Token*>* visitor) override;
    virtual void accept(Visitor<void>* visitor) override;
};

class This : public Expr
{
public:
    Token* keyword;

public:
    This(Token* _keyword);

    virtual std::string accept(Visitor<std::string>* visitor) override;
    virtual Token* accept(Visitor<Token*>* visitor) override;
    virtual void accept(Visitor<void>* visitor) override;
};

class Unary : public Expr
{
public:
    Token* optr;
    Expr* right;

public:
    Unary(Token* _optr, Expr* _right);

    virtual std::string accept(Visitor<std::string>* visitor) override;
    virtual Token* accept(Visitor<Token*>* visitor) override;
    virtual void accept(Visitor<void>* visitor) override;
};

class Variable : public Expr
{
public:
    Token* name;
    
public:
    Variable(Token* _name);

    virtual std::string accept(Visitor<std::string>* visitor) override;
    virtual Token* accept(Visitor<Token*>* visitor) override;
    virtual void accept(Visitor<void>* visitor) override;
};

// interface for visiting Exprs
template<class R>
class Expr::Visitor
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