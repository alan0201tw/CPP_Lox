#ifndef LOX_SCANNER
#define LOX_SCANNER

#include <iostream>
#include <vector>

#include "token.hpp"

class Scanner
{
private:
    std::string source;
    std::vector<Token> tokens;

    int start = 0;
    int current = 0;
    int line = 1;

    bool isAtEnd();
    void scanToken();
    char advance();
    void addToken(TokenType _type);
    void addToken(TokenType _type, Literal _literal);
    bool match(char _expected);

public:
    Scanner(std::string _source);
    std::vector<Token> scanTokens();
};

#endif