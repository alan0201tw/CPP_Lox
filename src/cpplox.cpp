#include <iostream>
#include "lox.hpp"

#include "expr.hpp"
#include "astPrinter.hpp"

/*
Program Entry for cpplox, a lox interpreter written in C++.
For conventions and styles, please refer to README.

By Shih "Alan" Yi-Chien
*/
int main(int argc, char const *argv[])
{
    /* AstPrinter testing
    Expr* expression = new Binary(
        new Unary(
            new Token(TokenType::MINUS, "-", nullptr, 1),
            new LiteralExpr(new Token(TokenType::NUMBER, "123", new Literal(123), 0))
        ),
        new Token(TokenType::STAR, "*", nullptr, 1),           
        new Grouping( new LiteralExpr(new Token(TokenType::NUMBER, "45.67", new Literal(45.67), 0))) );
    
    AstPrinter printer;

    std::cout << printer.print(expression) << std::endl;
    */

    if (argc == 1)
    {
        Lox::runPrompt();
    }
    else if (argc == 2)
    {
        /*
        argv[0] = the executable(cpplox)
        argv[1] = script file name
        */
        Lox::runFile(argv[1]);
    }
    else
    {
        std::cout << "Usage : cpplox [script]" << std::endl;
        // exit_code 64 : Usage error
        std::exit(64);
    }

    return 0;
}
