#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include "expr.hpp"
#include "stmt.hpp"
#include "environment.hpp"
#include "loxCallable.hpp"
#include "nativeFunction.hpp"

// forward-defining
class LoxCallable;

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

private:
    // Visit Expr methods
    Token* visitAssignExpr(Assign* expr);
    Token* visitBinaryExpr(Binary* expr);
    Token* visitCallExpr(Call* expr);
    Token* visitGetExpr(Get* expr) { throw std::logic_error("Function not yet implemented."); };
    Token* visitGroupingExpr(Grouping* expr);
    Token* visitLiteralExpr(LiteralExpr* expr);
    Token* visitLogicalExpr(Logical* expr);
    Token* visitSetExpr(Set* expr) { throw std::logic_error("Function not yet implemented."); };
    Token* visitSuperExpr(Super* expr) { throw std::logic_error("Function not yet implemented."); };
    Token* visitThisExpr(This* expr) { throw std::logic_error("Function not yet implemented."); };
    Token* visitUnaryExpr(Unary* expr);
    Token* visitVariableExpr(Variable* expr);

    // Visit Stmt methods
    void visitBlockStmt(Block* stmt);
    //void visitClassStmt(Class* stmt);
    void visitExpressionStmt(Expression* stmt);
    //void visitFunctionStmt(Function* stmt);
    void visitIfStmt(If* stmt);
    void visitPrintStmt(Print* stmt);
    //void visitReturnStmt(Return* stmt);
    void visitVarStmt(Var* stmt);
    void visitWhileStmt(While* stmt);

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

    Environment* const globals = new Environment();
    Environment* environment = globals;
};

#endif