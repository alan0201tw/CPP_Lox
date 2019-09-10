#ifndef LOX_ENVIRONMENT
#define LOX_ENVIRONMENT

#include <map>

#include "token.hpp"

class Environment
{
public:
    void define(std::string name, Token* value);
    Token* get(Token* name);
    Token* getAt(int distance, std::string name);
    void assign(Token* name, Token* value);
    void assignAt(int distance, Token* name, Token* value);

    Environment* ancestor(int distance);

    Environment();
    Environment(Environment* _enclosing);

    inline Environment* getEnclosing() { return enclosing; }

private:
    std::map<std::string, Token*> values;
    // a pointer to the environment that contains this environment
    Environment* enclosing;
};

#endif