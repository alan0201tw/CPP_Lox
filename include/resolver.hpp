#ifndef LOX_RESOLVER
#define LOX_RESOLVER

#include "expr.hpp"
#include "stmt.hpp"
#include "interpreter.hpp"

#include <vector>
#include <map>

enum class FunctionType : short
{
    NONE, FUNCTION, INITIALIZER, METHOD
};

enum class ClassType : short
{
    NONE, CLASS, SUBCLASS
};

class Resolver : public Expr::Visitor<void>, Stmt::Visitor<void>
{
private:
    typedef std::map<std::string, bool> stringToBoolMap;

    Interpreter* const interpreter;
    // Keys, as in Environment, are variable names.
    std::vector<stringToBoolMap*> scopes;
    
    FunctionType currentFunction = FunctionType::NONE; 
    ClassType currentClass = ClassType::NONE;

public:
    Resolver(Interpreter* _interpreter);

    virtual void visitBlockStmt(Block* stmt) override;
    virtual void visitVarStmt(Var* stmt) override;
    virtual void visitFunctionStmt(Function* stmt) override;
    virtual void visitExpressionStmt(Expression* stmt) override;
    virtual void visitIfStmt(If* stmt) override;
    virtual void visitPrintStmt(Print* stmt) override;
    virtual void visitReturnStmt(Return* stmt) override;
    virtual void visitWhileStmt(While* stmt) override;
    virtual void visitBreakStmt(Break* stmt) override;
    virtual void visitClassStmt(Class* stmt) override;

    virtual void visitVariableExpr(Variable* expr) override;
    virtual void visitAssignExpr(Assign* expr) override;
    virtual void visitBinaryExpr(Binary* expr) override;
    virtual void visitCallExpr(Call* expr) override;
    virtual void visitGroupingExpr(Grouping* expr) override;
    virtual void visitLiteralExpr(LiteralExpr* expr) override;
    virtual void visitLogicalExpr(Logical* expr) override;
    virtual void visitUnaryExpr(Unary* expr) override;

    virtual void visitGetExpr(Get* expr) override;// { throw std::logic_error("Function not yet implemented."); }
    virtual void visitSetExpr(Set* expr) override;// { throw std::logic_error("Function not yet implemented."); }
    virtual void visitSuperExpr(Super* expr) override;// { throw std::logic_error("Function not yet implemented."); }
    virtual void visitThisExpr(This* expr) override;// { throw std::logic_error("Function not yet implemented."); }

    void resolve(std::vector<Stmt*> _statements);

private:
    void resolve(Stmt* _statement);
    void resolve(Expr* _expression);

    void resolveLocal(Expr* _expr, Token* _name);
    void resolveFunction(Function* _function, FunctionType type);

    void declare(Token* _name);
    void define(Token* _name);

    void beginScope();
    void endScope();
};

#endif