#ifndef LOX_STMT
#define LOX_STMT

#include "expr.hpp"

class Stmt
{
public:
    // Forward Declaration of visitor interface
    template<class R> class Visitor;

    // very important! : initialize abstract methods with = 0 
    virtual void accept(Visitor<void>* visitor) = 0;
};

class Expression : public Stmt
{
public:
    Expr* expr;

    Expression(Expr* _expr);

    // define any accept methods needed for a specific type
    virtual void accept(Visitor<void>* visitor) override;
};

class Print : public Stmt
{
public:
    Expr* expr;

    Print(Expr* _expr);

    virtual void accept(Visitor<void>* visitor) override;
};

class Var : public Stmt
{
public:
    Token* name;
    Expr* initializer;

    Var(Token* _name, Expr* _initializer);

    virtual void accept(Visitor<void>* visitor) override;
};

class Block : public Stmt
{
public:
    std::vector<Stmt*> statements;

    Block(std::vector<Stmt*> _statements);

    virtual void accept(Visitor<void>* visitor) override;
};

class If : public Stmt
{
public:
    Expr* condition;
    Stmt* thenBranch;
    Stmt* elseBranch;

    If(Expr* _condition, Stmt* _thenBranch, Stmt* _elseBranch);

    virtual void accept(Visitor<void>* visitor) override;
};

class While : public Stmt
{
public:
    Expr* condition;
    Stmt* body;

    While(Expr* _condition, Stmt* _body);

    virtual void accept(Visitor<void>* visitor) override;    
};

class Function : public Stmt
{
public:
    Token* name;
    std::vector<Token*> params;
    std::vector<Stmt*> body;

    Function(Token* _name, std::vector<Token*> _params, std::vector<Stmt*> _body);

    virtual void accept(Visitor<void>* visitor) override;    
};

// definition of the nestes class
template<class R> class Stmt::Visitor
{
public:
    virtual R visitBlockStmt(Block* stmt) = 0;
    //virtual R visitClassStmt(Class* stmt) = 0;
    virtual R visitExpressionStmt(Expression* stmt) = 0;
    virtual R visitFunctionStmt(Function* stmt) = 0;
    virtual R visitIfStmt(If* stmt) = 0;
    virtual R visitPrintStmt(Print* stmt) = 0;
    //virtual R visitReturnStmt(Return* stmt) = 0;
    virtual R visitVarStmt(Var* stmt) = 0;
    virtual R visitWhileStmt(While* stmt) = 0;
};

#endif