#include "scanner.hpp"

#include "lox.hpp"

std::map<std::string, TokenType> Scanner::keywords = 
{
    {"and", TokenType::AND},
    {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},
    {"false", TokenType::FALSE},
    {"for", TokenType::FOR},
    {"fun", TokenType::FUN},
    {"if", TokenType::IF},
    {"nil", TokenType::NIL},
    {"or", TokenType::OR},
    {"print", TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super", TokenType::SUPER},
    {"this", TokenType::THIS},
    {"true", TokenType::TRUE},
    {"var", TokenType::VAR},
    {"while", TokenType::WHILE},
    //
    {"break", TokenType::BREAK},
};

Scanner::Scanner(std::string _source)
{
    source = _source;
}

std::vector<Token*> Scanner::scanTokens()
{
    while(!isAtEnd())
    {
        // at the beginning of the next lexeme
        start = current;
        scanToken();
    }

    tokens.push_back(new Token(TokenType::EOF_TOKEN, "", nullptr, line));
    return tokens;
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

        case '/':
            if(match('/')) // a single line comment
                while(peek() != '\n' && !isAtEnd()) advance();
            else
                addToken(TokenType::SLASH);
            break;

        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace, carriage return, and tab
            break;
        
        // when reading a new line character, ignore it and update the line count
        case '\n':
            line++;
            break;

        case '"': string(); break;

        default : 
            if(isdigit(c))
            {
                number();
            }
            else if(isalpha(c))
            {
                identifier();
            }
            else
            {
                std::string message = "Unexpected character : ";
                message += c;
                Lox::error(line, message);
            }
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
    addToken(_type, nullptr);
}

void Scanner::addToken(TokenType _type, Literal* _literal)
{
    std::string text = source.substr(start, current - start);
    tokens.push_back(new Token(_type, text, _literal, line));
}

bool Scanner::match(char _expected)
{
    if (isAtEnd()) return false;
    if (source[current] != _expected) return false;

    current++;
    return true;
}

char Scanner::peek()
{
    if (isAtEnd()) return '\0';   
    return source[current];
}

char Scanner::peekNext()
{
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

void Scanner::string()
{
    std::string value = "";

    while(peek() != '"' && !isAtEnd())
    {
        if(peek() == '\n')
            line++;
        // append the char to the string
        value += advance();
    }

    // hanging string, not-finished
    if(isAtEnd())
    {
        Lox::error(line, "Unterminated string.");
        return;
    }

    // the closing '"', char checking is done in the initial loop, it'll break when current
    // char is '"' or at end, since the latter case is resolved above, we can safely assume
    // current char is '"'
    advance();

    Literal* literal = new Literal(value);

    addToken(TokenType::STRING, literal);
}

void Scanner::number()
{
    while(isdigit(peek())) advance();

    // look for floating point
    if(peek() == '.' && isdigit(peekNext()))
    {
        // consume '.'
        advance();

        while(isdigit(peek())) advance();
    }
    Literal* literal = new Literal(std::stod(source.substr(start, current - start)));

    addToken(TokenType::NUMBER, literal);
}

void Scanner::identifier()
{
    std::string value;

    while (isalnum(peek())) advance();

    TokenType type;
    
    value = source.substr(start, current - start);

    if(keywords.find(value) != keywords.end())
    {
        type = keywords[value];
    }
    else
    {
        type = TokenType::IDENTIFIER;
    }

    addToken(type);
}