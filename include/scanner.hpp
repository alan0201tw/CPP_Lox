#ifndef LOX_SCANNER
#define LOX_SCANNER

#include <iostream>
#include <vector>
#include <map>

#include "token.hpp"

class Scanner
{
private:

    static std::map<std::string, TokenType> keywords;

    std::string source;
    std::vector<Token> tokens;

    size_t start = 0;
    size_t current = 0;
    size_t line = 1;

    bool isAtEnd();
    void scanToken();
    char advance();
    void addToken(TokenType _type);
    void addToken(TokenType _type, Literal* _literal);
    bool match(char _expected);
    char peek();
    char peekNext();

    void string();
    void number();
    void identifier();

public:
    Scanner(std::string _source);
    std::vector<Token> scanTokens();
};

#endif