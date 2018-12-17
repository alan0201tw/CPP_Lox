#ifndef LOX_ENVIRONMENT
#define LOX_ENVIRONMENT

#include <iostream>
#include <map>

#include "token.hpp"

class Environment
{
public:
    void define(std::string name, Token* value);
    Token* get(Token* name);
    void assign(Token* name, Token* value);

private:
    std::map<std::string, Token*> values;
};

#endif