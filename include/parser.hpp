#ifndef LOX_PARSER
#define LOX_PARSER

#include <vector>

#include "token.hpp"
#include "expr.hpp"

#include "loxException.hpp"

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
    Expr* parse();
};

#endif