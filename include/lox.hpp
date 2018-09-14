#ifndef CPPLOX
#define CPPLOX

#include <iostream>
#include "token.hpp"

class Lox
{
public:
    // static fields need to be initialized outside the class, in this case
    // in the src file of Lox class -> lox.cpp
    static bool hadError;

    static void runFile(const char* file_name);
    static void runPrompt();
    // for logging error messages
    static void error(int line, std::string message);
    // for logging errors in Parsing process
    static void error(Token* _token, std::string _message);
private:
    static void run(std::string source);
    // report
    static void report(int line, std::string where, std::string message);
};

#endif