#include "parser.hpp"

#include <exception>
#include "lox.hpp"

Parser::Parser(std::vector<Token*> _tokens)
{
    tokens = _tokens;
}

Expr* Parser::expression()
{
    return equality();
}

Expr* Parser::equality()
{
    Expr* expr = comparison();

    std::vector<TokenType> equalityOptrTokenTypes = {TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL};
    // if the current(unprocessed) token is of type == or !=, advance and form a chain of equality
    while(match(equalityOptrTokenTypes))
    {
        // since match() will advance the tokens, the == or != operator will be at (current - 1)
        Token* optr = previous();
        Expr* right = comparison();
        expr = new Binary(expr, optr, right);
    }

    return expr;
}

Expr* Parser::comparison()
{
    Expr* expr = addition();

    std::vector<TokenType> comparisonOptrTokenTypes = 
    {TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL};

    while(match(comparisonOptrTokenTypes))
    {
        Token* optr = previous();
        Expr* right = addition();
        expr = new Binary(expr, optr, right);
    }

    return expr;
}

Expr* Parser::addition()
{
    Expr* expr = multiplication();

    std::vector<TokenType> additionOptrTokenTypes = {TokenType::MINUS, TokenType::PLUS};
    while (match(additionOptrTokenTypes))
    {                    
        Token* optr = previous();                
        Expr* right = multiplication();               
        expr = new Binary(expr, optr, right);
    }                                               

    return expr;
}

Expr* Parser::multiplication()
{
    Expr* expr = unary();

    std::vector<TokenType> multiplicationOptrTokenTypes = {TokenType::SLASH, TokenType::STAR};
    while (match(multiplicationOptrTokenTypes))
    {                    
        Token* optr = previous();                
        Expr* right = unary();               
        expr = new Binary(expr, optr, right);
    }                                               

    return expr;
}

Expr* Parser::unary()
{
    std::vector<TokenType> unaryOptrTokenTypes = {TokenType::BANG, TokenType::MINUS};

    if(match(unaryOptrTokenTypes))
    {
        Token* optr = previous();
        Expr* right = unary();
        return new Unary(optr, right);
    }

    return primary();
}

Expr* Parser::primary()
{
    // in cpplox, instead of the master base class of Object in other languages, we use a union object for sotring
    // different values, in this case we store bool value false as a double value of false in cpp.
    if(match({TokenType::FALSE})) return new LiteralExpr(new Token(TokenType::FALSE, "false", new Literal(false), 0));
    if(match({TokenType::TRUE})) return new LiteralExpr(new Token(TokenType::TRUE, "true", new Literal(true), 0));
    if(match({TokenType::NIL})) return new LiteralExpr(new Token(TokenType::NIL, "nil", new Literal(nullptr), 0));

    if(match({TokenType::NUMBER, TokenType::STRING}))
    {
        // in jlox, it contains only the literal of previous token
        return new LiteralExpr(previous());
    }

    if(match({TokenType::LEFT_PAREN}))
    {
        Expr* expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
        return new Grouping(expr);
    }

    throw error(peek(), "Expect expression.");
}

// utility methods

bool Parser::match(std::vector<TokenType> _types)
{
    for(size_t i = 0;i < _types.size();i++)
    {
        if(check(_types[i]))
        {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type)
{
    if (isAtEnd()) return false;         
    return peek()->type == type;          
}

Token* Parser::advance()
{   
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd()
{
    return peek()->type == TokenType::EOF_TOKEN;
}

Token* Parser::peek()
{           
    return tokens[current];    
}

Token* Parser::previous()
{       
    return tokens[current - 1];
}

Token* Parser::consume(TokenType _type, std::string _message)
{
    if(check(_type)) return advance();

    throw error(peek(), _message);
}

ParseError* Parser::error(Token* _token, std::string _message)
{
    Lox::error(_token, _message);
    return new ParseError();
}

void Parser::synchronize()
{
    // this method is suppose to jump of the statement where an error occurs to avoid false reporting error chains
    // what it does is trying to find where the next statement starts and restart parsing there

    advance();

    while (!isAtEnd())
    {                       
        if (previous()->type == TokenType::SEMICOLON) return;

        switch (peek()->type)
        {
        case TokenType::CLASS:
        case TokenType::FUN:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::PRINT:
        case TokenType::RETURN:
            return;
            break;
        default:
            break;
        }

        advance();
    }
}

// The main parse method

Expr* Parser::parse()
{
    try
    {
        return expression();
    }
    catch(ParseError& e)
    {
        return nullptr;
    }
}