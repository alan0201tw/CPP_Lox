#include "lox.hpp"

#include <iostream>
#include <fstream>
#include <vector>

#include "token.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "astPrinter.hpp"

// initializing static fields
bool Lox::hadError = false;
bool Lox::hadRuntimeError = false;
Interpreter* Lox::interpreter = new Interpreter();

void Lox::runFile(const char* file_name)
{
    std::ifstream inputStream(file_name, std::ios::in | std::ios::binary);

    // check if inputFile opens correctly
    if(inputStream)
    {
        std::string content;
        inputStream.seekg(0, std::ios::end);
        content.resize(inputStream.tellg());
        inputStream.seekg(0, std::ios::beg);
        inputStream.read(&content[0], content.size());
        inputStream.close();

        run(content);

        if(hadError == true)
        {
            std::exit(65);
        }
        if(hadRuntimeError == true)
        {
            std::exit(70);
        }

        return;
    }
    throw(errno);
};

void Lox::runPrompt()
{
    std::string input;

    std::cout << "Running prompt... use ctrl+c to exit..." << std::endl;

    while(true)
    {
        std::getline(std::cin, input);
        // here we need to specify that we are running under REPL
        run(input, true);
        hadError = false;
    }
}

void Lox::run(std::string source, bool isREPL)
{
    // std::cout << "running source = " << source << std::endl;

    Scanner scanner(source);
    std::vector<Token*> tokens = scanner.scanTokens();

    // add parsing
    Parser* parser = new Parser(tokens);
    std::vector<Stmt*> statements = parser->parse();

    // Stop if there was a syntax error.
    if(hadError) return;
    
    if(isREPL == false)
    {
        interpreter->interpret(statements);
    }
    else
    {
        interpreter->interpret_REPL(statements);
    }
}

void Lox::error(int line, std::string message)
{
    report(line,"",message);
}

void Lox::error(Token* _token, std::string _message)
{
    if(_token->type == TokenType::EOF_TOKEN)
    {
        report(_token->line, " at end", _message);
    }
    else
    {
        std::string where = " at '" + _token->lexeme + "'";
        report(_token->line, where, _message);
    }
}

void Lox::runtimeError(RuntimeError* error)
{
    std::cerr << error->what() << "\n[line" << error->token->line << "]" << std::endl;
    hadRuntimeError = true;
}

void Lox::report(int line, std::string where, std::string message)
{
    std::cerr << "[line " << line << + "] Error" << where << ": " << message << std::endl;
    hadError = true;
}