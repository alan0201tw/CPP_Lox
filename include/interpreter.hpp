#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include "expr.hpp"
#include "stmt.hpp"

#include <map>

// forward-defining
class LoxCallable;
class Environment;
class LoxClass;

/**
 * in jlox, the return type of the visitor is a Object, which can be any type in Java.
 * in cpplox, I use the Token class for this since it contains every information we need to form a object, with
 * some extra fields that can be left empty. ( line, lexeme )
 * Also, tokens for this purpose should be within these types :
 * NUMBER, STRING, TRUE, FALSE. We need these types to represent double, string, bool values.
 */
class Interpreter : public Expr::Visitor<Token*>, Stmt::Visitor<void>
{
public:
    Interpreter();

    void interpret(std::vector<Stmt*> statements);
    // used only in REPL, for printing values of expressions
    void interpret_REPL(std::vector<Stmt*> statements);

    // for resolvers
    void resolve(Expr* _expr, int depth);

private:
    // Visit Expr methods
    virtual Token* visitAssignExpr(Assign* expr) override;
    virtual Token* visitBinaryExpr(Binary* expr) override;
    virtual Token* visitCallExpr(Call* expr) override;
    virtual Token* visitGetExpr(Get* expr) override;// { throw std::logic_error("Function not yet implemented."); };
    virtual Token* visitGroupingExpr(Grouping* expr) override;
    virtual Token* visitLiteralExpr(LiteralExpr* expr) override;
    virtual Token* visitLogicalExpr(Logical* expr) override;
    virtual Token* visitSetExpr(Set* expr) override;// { throw std::logic_error("Function not yet implemented."); };
    virtual Token* visitSuperExpr(Super* expr) override;// { throw std::logic_error("Function not yet implemented."); };
    virtual Token* visitThisExpr(This* expr) override;// { throw std::logic_error("Function not yet implemented."); };
    virtual Token* visitUnaryExpr(Unary* expr) override;
    virtual Token* visitVariableExpr(Variable* expr) override;

    Token* lookUpVariable(Token* _name, Expr* _expr);

    // Visit Stmt methods
    virtual void visitBlockStmt(Block* stmt) override;
    virtual void visitClassStmt(Class* stmt) override;
    virtual void visitExpressionStmt(Expression* stmt) override;
    virtual void visitFunctionStmt(Function* stmt) override;
    virtual void visitIfStmt(If* stmt) override;
    virtual void visitPrintStmt(Print* stmt) override;
    virtual void visitReturnStmt(Return* stmt) override;
    virtual void visitVarStmt(Var* stmt) override;
    virtual void visitWhileStmt(While* stmt) override;
    //
    virtual void visitBreakStmt(Break* stmt) override;

private:
    Token* evaluate(Expr* expr);
    bool isTruthy(Token* _token);
    bool isEqual(Token* a, Token* b);

    // hepler methods for statements
    void execute(Stmt* _stmt);
    void executeBlock(std::vector<Stmt*> _statements, Environment* _environment);
    
    // runtime error detection and checking
    void checkNumberOperand(Token* _optr, Token* _operand);
    void checkNumberOperands(Token* _optr, Token* _left, Token* _right);

    // my additional utility methods
    static Token* boolToken(bool _value); // return a Token representing given bool value
    static Token* doubleToken(double _value); // return a Token representing given double value
    static Token* stringToken(std::string _value); // return a Token representing given string value
    static Token* callableToken(LoxCallable* _callable); // return a Token representing a LoxCallable
    static Token* classToken(LoxClass* _class);

    Environment* const globals;
    Environment* environment;

    std::map<Expr*, int> locals;
    
    friend class LoxFunction;
};

#endif