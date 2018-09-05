#ifndef LOX_TOKEN
#define LOX_TOKEN

#include <iostream>
#include <sstream>
// std::memset
#include <cstring>

enum class TokenType : char
{
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    // avoid naming EOF since it's a defined macro in many libs
    EOF_TOKEN
};


union Literal
{
    // for lox, only string and double value.
    // in other language, you might want to add integer value support.
    std::string stringValue;
    double doubleValue;

    Literal();
    Literal(double _doubleValue);
    Literal(std::string _stringValue);

    ~Literal() {}
};

class Token final
{
private:
    TokenType type;
    std::string lexeme;
    Literal* literal;
    int line;

public:
    Token(TokenType _type, std::string _lexeme, Literal* _literal, int _line);
    ~Token() {}

    std::string toString();
};

#endif