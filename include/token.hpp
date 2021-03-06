#ifndef LOX_TOKEN
#define LOX_TOKEN

#include <iostream>

#include "loxCallable.hpp"

class LoxClass;
class LoxInstance;

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
    // added Literals.
    CALLABLE, INSTANCE,

    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
    // added Keywords.
    BREAK,

    // avoid naming EOF since it's a defined macro in many libs
    EOF_TOKEN
};

union Literal
{
    // for lox, only string and double value.
    // in other language, you might want to add integer value support.
    std::string stringValue;
    double doubleValue;
    // use Literals to store LoxCallables
    LoxCallable* callableValue;
    LoxClass* classValue;
    LoxInstance* instanceValue;

    explicit Literal();
    explicit Literal(double _doubleValue);
    explicit Literal(std::string _stringValue);
    explicit Literal(LoxCallable* _callableValue);
    explicit Literal(LoxClass* _classValue);
    explicit Literal(LoxInstance* _instanceValue);

    ~Literal() {}
};

class Token final
{
public:
    TokenType type;
    std::string lexeme;
    Literal* literal;
    int line;

public:
    explicit Token(TokenType _type, std::string _lexeme, Literal* _literal, int _line);
    ~Token() {}

    std::string toString();
};

#endif