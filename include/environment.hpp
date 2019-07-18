#ifndef LOX_ENVIRONMENT
#define LOX_ENVIRONMENT

#include <map>

#include "token.hpp"

class Environment
{
public:
    void define(std::string name, Token* value);
    Token* get(Token* name);
    void assign(Token* name, Token* value);

    Environment();
    Environment(Environment* _enclosing);

private:
    std::map<std::string, Token*> values;
    // a pointer to the environment that contains this environment
    Environment* enclosing;
};

#endif