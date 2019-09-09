#include "loxException.hpp"

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
    //Expr* expr = equality();
    Expr* expr = logic_or();

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
        else if(Get* get = dynamic_cast<Get*>(expr))
        {
            return new Set(get->object, get->name, value);
        }
        // if the l-value is not a Variable, throw error
        error(equals, "Invalid assignment target.");
    }

    return expr;
}

Expr* Parser::logic_or()
{
    Expr* expr = logic_and();

    while(match({TokenType::OR}))
    {
        Token* optr = previous();
        Expr* right = logic_and();
        expr = new Logical(expr, optr, right);
    }

    return expr;
}

Expr* Parser::logic_and()
{
    Expr* expr = equality();

    while(match({TokenType::AND}))
    {
        Token* optr = previous();
        Expr* right = equality();
        expr = new Logical(expr, optr, right);
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

    // replace this line for function calls
    //return primary();

    return call();
}

Expr* Parser::call()
{
    Expr* expr = primary();
    
    while(true)
    {
        if(match({TokenType::LEFT_PAREN}))
        {
            expr = finishCall(expr);
        }
        // added for calling on instances
        else if(match({TokenType::DOT}))
        {
            Token* name = consume(TokenType::IDENTIFIER, "Expect property name after '.'.");
            expr = new Get(expr, name);
        }
        else
        {
            break;
        }
    }

    return expr;
}

Expr* Parser::finishCall(Expr* callee)
{
    std::vector<Expr*> arguments;
    if(!check(TokenType::RIGHT_PAREN))
    {
        do
        {
            // check the number of arguments passing to the function
            // note that we do not throw the returned exception from error()
            // this is because we do not want the interpreter to enter panic state
            // the intepreter is stll valid, it's just the user need t be informed
            // that the argument count should not be this long
            if(arguments.size() >= 8)
            {
                error(peek(), "Cannot have more than 8 arguments.");
            }

            arguments.push_back(expression());
        } while (match({TokenType::COMMA}));
    }

    Token* closingParen = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");

    return new Call(callee, closingParen, arguments);
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

    if(match({TokenType::THIS}))
    {
        return new This(previous());
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
    if(match({TokenType::FOR})) return forStatement();
    if(match({TokenType::IF})) return ifStatement();
    if(match({TokenType::PRINT})) return printStatement();
    if(match({TokenType::RETURN})) return returnStatement();
    if(match({TokenType::WHILE})) return whileStatement();
    if(match({TokenType::LEFT_BRACE})) return new Block(block());
    // added for break statement
    if(match({TokenType::BREAK})) return breakStatement();

    return expressionStatement();
}

Stmt* Parser::forStatement()
{
    consume(TokenType::LEFT_PAREN, "Expext '(' after 'for'.");
    // obtaining the initializer
    Stmt* initializer;
    if(match({TokenType::SEMICOLON}))
    {
        initializer = nullptr;
    }
    else if(match({TokenType::VAR}))
    {
        initializer = varDeclaration();
    }
    else
    {
        // if the initializer is not empty and not var declaration.
        // then by default it is an expression.
        initializer = expressionStatement();
    }

    // obtaining the condition
    Expr* condition = nullptr;
    if(!check(TokenType::SEMICOLON))
    {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after for loop condition.");

    // the increment expression
    Expr* increment = nullptr;
    if(!check(TokenType::RIGHT_PAREN))
    {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

    // finally, get the content of the for loop
    Stmt* body = statement();

    // Desugaring
    if(increment != nullptr)
    {
        // for(...;...; <increment> ) { <body> }
        // 
        // can be translated to 
        //
        // while(...) { <body> <increment> }
        //
        body = new Block({
            body,
            new Expression(increment)
        });
    }

    // if there is no stopping condition, replace it with a inf-loop
    if(condition == nullptr)
    {
        condition = new LiteralExpr(new Token(TokenType::TRUE, "true", nullptr, 0));
    }
    body = new While(condition, body);

    if(initializer != nullptr)
    {
        body = new Block({
            initializer,
            body
        });
    }

    // TODO : add "break" support to Lox
    
    return body;
}

Stmt* Parser::ifStatement()
{
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    Expr* condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

    Stmt* thenBranch = statement();
    Stmt* elseBranch = nullptr;
    if(match({TokenType::ELSE}))
    {
        elseBranch = statement();
    }

    return new If(condition, thenBranch, elseBranch);
}

Stmt* Parser::printStatement()
{
    Expr* value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return new Print(value);
}

Stmt* Parser::returnStatement()
{
    Token* keyword = previous();
    Expr* value = nullptr;

    if(!check(TokenType::SEMICOLON))
    {
        value = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return new Return(keyword, value);
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
        // added for enabling function declaration
        if(match({TokenType::CLASS})) return classDeclaration();
        if(match({TokenType::FUN})) return function("function");
        if(match({TokenType::VAR})) return varDeclaration();

        return statement();
    }
    catch(ParseError* error)
    {
        synchronize();
        return nullptr;
    }
}

Stmt* Parser::whileStatement()
{
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");   
    Expr* condition = expression();                      
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    Stmt* body = statement();

    return new While(condition, body);
}

Stmt* Parser::breakStatement()
{
    consume(TokenType::SEMICOLON, "Expected ';' after 'break'.");
    return new Break();
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

Stmt* Parser::classDeclaration()
{
    Token* name = consume(TokenType::IDENTIFIER, "Expect class name.");
    // add support for detecting inheritance
    Variable* superclass = nullptr;
    if(match({TokenType::LESS}) == true)
    {
        consume(TokenType::IDENTIFIER, "Expect superclass name.");
        superclass = new Variable(previous());
    }
    //
    consume(TokenType::LEFT_BRACE, "Expect '{' before class body.");
    
    std::vector<Function*> methods;
    while(!check(TokenType::RIGHT_BRACE) && !isAtEnd())
    {
        methods.push_back(function("method"));
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after class body.");

    // TODO : change the nullptr field
    return new Class(name, superclass, methods);
}

Function* Parser::function(std::string kind)
{
    Token* name = consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");
    consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token*> parameters;

    if(!check(TokenType::RIGHT_PAREN))
    {
        do
        {
            if(parameters.size() >= 8)
            {
                error(peek(), "Cannot have more than 8 parameters.");
            }

            parameters.push_back(consume(TokenType::IDENTIFIER, "Expect parameter name."));
        } while (match({TokenType::COMMA}));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters");

    consume(TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<Stmt*> body = block();
    return new Function(name, parameters, body);
}

// This function will NOT consume the left brace and will stop 
// after consuming the right brace.
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

    // TODO :
    // when an consume cannot be satisfied, it can be caused by REPL.
    // in this case, we should allow user to continue the input.
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