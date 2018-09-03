#include <iostream>
#include "lox.hpp"

/*
Program Entry for cpplox, a lox interpreter written in C++.
For conventions and styles, please refer to README.

By Shih "Alan" Yi-Chien
*/
int main(int argc, char const *argv[])
{
    if(argc == 1)
    {
        Lox::runPrompt();
    }
    else if(argc == 2)
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
