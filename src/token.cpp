#include "token.hpp"

Token::Token(TokenType _type, std::string _lexeme, Literal* _literal, int _line)
{
    type = _type;
    lexeme = _lexeme;
    line = _line;
    literal = _literal;
}

std::string Token::toString()
{
    std::stringstream s;
    s << "Type : " << (short)type << ", Lexeme : " << lexeme << ", Literal : " ;
    // switch on type tag
    if(type == TokenType::NUMBER)
    {
        s << literal->doubleValue;
    }
    else if(type == TokenType::STRING)
    {
        s << literal->stringValue;

        std::cout << "literal->stringValue = " << literal->stringValue << std::endl;
    }
    
    return s.str();
}