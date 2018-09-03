#include "scanner.hpp"

#include "lox.hpp"

Scanner::Scanner(std::string _source)
{
    source = _source;
}

std::vector<Token> Scanner::scanTokens()
{
    while(!isAtEnd())
    {
        // at the beginning of the next lexeme
        start = current;
        scanToken();
    }

    tokens.push_back(Token(TokenType::EOF_TOKEN, "", Literal(), line));
}

bool Scanner::isAtEnd()
{
    return current >= source.length();
}

void Scanner::scanToken()
{
    char c = advance();
    switch(c)
    {
        case '(' : addToken(TokenType::LEFT_PAREN); break;
        case ')' : addToken(TokenType::RIGHT_PAREN); break;
        case '{' : addToken(TokenType::LEFT_BRACE); break;
        case '}' : addToken(TokenType::RIGHT_BRACE); break;
        case ',' : addToken(TokenType::COMMA); break;
        case '.' : addToken(TokenType::DOT); break;
        case '-' : addToken(TokenType::MINUS); break;
        case '+' : addToken(TokenType::PLUS); break;
        case ';' : addToken(TokenType::SEMICOLON); break;
        case '*' : addToken(TokenType::STAR); break;

        case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;      
        case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;    
        case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;      
        case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;

        default : 
            Lox::error(line, "Unexpected character.");
            break;
    }
}

char Scanner::advance()
{
    // increase current and return previous char, current char is unprocessed
    current++;
    return source[current - 1];
}

void Scanner::addToken(TokenType _type)
{
    // since the actual literal should be irrelevant, we can just insert a
    addToken(_type, Literal());
}

void Scanner::addToken(TokenType _type, Literal _literal)
{
    std::string text = source.substr(start, current - start);
    tokens.push_back(Token(_type, text, _literal, line));
}

bool Scanner::match(char _expected)
{
    if (isAtEnd()) return false;
    if (source[current] != _expected) return false;

    current++;
    return true;
}