#include <exception>

#include "parser.hpp"
#include "lox.hpp"


Parser::Parser(std::vector<Token*> _tokens)
{
    tokens = _tokens;
}

Expr* Parser::expression()
{
    return assignment();
}

Expr* Parser::assignment()
{
    // the l-value, this should be of type Variable(a sub-class of Expr)
    // if not, it's propably syntax error.
    Expr* expr = equality();

    if(match({TokenType::EQUAL}))
    {
        Token* equals = previous();
        Expr* value = assignment();

        // try to cast expr to a variable sub-class
        if(Variable* var = dynamic_cast<Variable*>(expr))
        {
            Token* name = var->name;
            //std::cout << name << " " << value << std::endl;
            return new Assign(name, value);
        }
        // if the l-value is not a Variable, throw error
        error(equals, "Invalid assignment target.");
    }

    return expr;
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
    // in cpplox, instead of the master base class of Object in other languages, we use a union object for storing
    // different values, in this case we store bool value false as a double value of false in cpp.
    if(match({TokenType::FALSE})) return new LiteralExpr(new Token(TokenType::FALSE, "false", nullptr, 0));
    if(match({TokenType::TRUE})) return new LiteralExpr(new Token(TokenType::TRUE, "true", nullptr, 0));
    if(match({TokenType::NIL})) return new LiteralExpr(new Token(TokenType::NIL, "nil", nullptr, 0));

    if(match({TokenType::NUMBER, TokenType::STRING}))
    {
        // in jlox, it contains only the literal of previous token
        // like : return new Expr.Literal(previous().literal);
        return new LiteralExpr(previous());
    }

    if(match({TokenType::IDENTIFIER}))
    {
        return new Variable(previous());
    }

    if(match({TokenType::LEFT_PAREN}))
    {
        Expr* expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
        return new Grouping(expr);
    }

    throw error(peek(), "Expect expression.");
}

// statement methods

Stmt* Parser::statement()
{
    if(match({TokenType::PRINT})) return printStatement();
    if(match({TokenType::LEFT_BRACE})) return new Block(block());

    return expressionStatement();
}

Stmt* Parser::printStatement()
{
    Expr* value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return new Print(value);
}

Stmt* Parser::expressionStatement()
{
    Expr* expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return new Expression(expr);
}

Stmt* Parser::declarationStatement()
{
    try
    {
        if(match({TokenType::VAR})) return varDeclaration();

        return statement();
    }
    catch(ParseError* error)
    {
        synchronize();
        return nullptr;
    }
}

Stmt* Parser::varDeclaration()
{
    Token* name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    Expr* initializer = nullptr;
    if (match({TokenType::EQUAL}))
    {
        initializer = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return new Var(name, initializer);
}

std::vector<Stmt*> Parser::block()
{
    std::vector<Stmt*> statements;

    while(check(TokenType::RIGHT_BRACE) == false && !isAtEnd())
    {
        statements.push_back(declarationStatement());
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
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

std::vector<Stmt*> Parser::parse()
{
    std::vector<Stmt*> statements;
    while(!isAtEnd())
    {
        statements.push_back(declarationStatement());

        //statements.push_back(statement());
    }

    return statements;
}