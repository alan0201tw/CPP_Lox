#include "lox.hpp"

#include <iostream>
#include <fstream>
#include <vector>

#include "token.hpp"
#include "scanner.hpp"

// initializing static fields
bool Lox::hadError = false;

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
        run(input);
        hadError = false;
    }
}

void Lox::run(std::string source)
{
    // std::cout << "running source = " << source << std::endl;

    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    for(size_t i=0; i < tokens.size(); i++)
    {
        std::cout << "Token " << i << " : " << tokens[i].toString() << std::endl;
    }
}

void Lox::error(int line, std::string message)
{
    report(line,"",message);
}

void Lox::report(int line, std::string where, std::string message)
{
    std::cerr << "[line " << line << + "] Error" << where << ": " << message << std::endl;
    hadError = true;
}