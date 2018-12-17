#ifndef LOX_PARSER
#define LOX_PARSER

#include <vector>

#include "token.hpp"
#include "expr.hpp"
#include "loxException.hpp"
#include "stmt.hpp"

class Parser
{
private:
    std::vector<Token*> tokens;
    int current = 0;

    // grammar rule methods, each rule returns a corresponding grammar variable
    Expr* expression();
    Expr* equality();
    Expr* comparison();
    Expr* addition();
    Expr* multiplication();
    Expr* unary();
    Expr* primary();

    // added rule for statements
    Stmt* statement();
    Stmt* printStatement();
    Stmt* expressionStatement();
    Stmt* declarationStatement();

    Stmt* varDeclaration();

    // utility methods

    // return true if the current token is any of the given types
    bool match(std::vector<TokenType> _types);
    bool check(TokenType _type);
    Token* advance();
    bool isAtEnd();
    Token* peek();
    Token* previous();
    Token* consume(TokenType _type, std::string _message);

    // for reporting error and synchronizing( to bestly report futher errors after the first one )
    ParseError* error(Token* _token, std::string _message);
    void synchronize();

public:
    Parser(std::vector<Token*> _tokens);
    std::vector<Stmt*> parse();
};

#endif