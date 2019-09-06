#include "token.hpp"
#include "loxFunction.hpp"
#include "loxClass.hpp"
#include "loxInstance.hpp"

// std::memset
#include <cstring>
#include <sstream>

Literal::Literal()
{
    std::memset(this, 0, sizeof(Literal));
}

Literal::Literal(double _doubleValue) : Literal()
{
    doubleValue = _doubleValue;
}

Literal::Literal(std::string _stringValue) : Literal()
{
    stringValue = _stringValue;
}

Literal::Literal(LoxCallable* _callableValue) : Literal()
{
    callableValue = _callableValue;
}

Literal::Literal(LoxClass* _classValue) : Literal()
{
    classValue = _classValue;
}

Literal::Literal(LoxInstance* _instanceValue) : Literal()
{
    instanceValue = _instanceValue;
}

Token::Token(TokenType _type, std::string _lexeme, Literal *_literal, int _line)
{
    type = _type;
    lexeme = _lexeme;
    line = _line;
    literal = _literal;
}

std::string Token::toString()
{
    std::stringstream s;
    // uncomment this line for debug info
    //s << "Type : " << (short)type << ", Lexeme : " << lexeme << ", Literal : " ;
    // switch on type tag
    if (literal == nullptr)
    {
        s << "null";
    }
    else if (type == TokenType::NUMBER)
    {
        s << literal->doubleValue;
    }
    else if (type == TokenType::STRING)
    {
        s << literal->stringValue;
    }
    else if(type == TokenType::CALLABLE)
    {
        LoxFunction* tmp = dynamic_cast<LoxFunction*>(literal->callableValue);
        
        if(tmp == nullptr)
        {
            s << "None LoxFunction callable";
        }
        else
        {
            s << "<fn " << tmp->declaration->name->lexeme << ">";
        }
    }
    else if(type == TokenType::CLASS)
    {
        LoxClass* tmp = dynamic_cast<LoxClass*>(literal->classValue);
        s << tmp->ToString();
    }
    else if(type == TokenType::INSTANCE)
    {
        LoxInstance* tmp = dynamic_cast<LoxInstance*>(literal->instanceValue);
        s << tmp->ToString();
    }

    return s.str();
}