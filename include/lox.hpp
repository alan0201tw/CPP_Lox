#ifndef CPPLOX
#define CPPLOX

#include <iostream>

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
private:
    static void run(std::string source);
    // report
    static void report(int line, std::string where, std::string message);
};

#endif