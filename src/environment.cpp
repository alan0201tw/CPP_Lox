#include "environment.hpp"

#include "loxException.hpp"

Environment::Environment()
{
    enclosing = nullptr;
}

Environment::Environment(Environment* _enclosing)
{
    enclosing = _enclosing;
}

void Environment::define(std::string name, Token* value)
{
    delete(values[name]);
    values[name] = value;
};

Token* Environment::get(Token* name)
{
    if(values.find(name->lexeme) != values.end())
    {
        return values[name->lexeme];
    }
    // if the wanted variable cannot be found in the current environment,
    // look up in the enclosing environment
    if(enclosing != nullptr)
    {
        return enclosing->get(name);
    }

    throw new RuntimeError(name, "Undefined variable \'" + name->lexeme + "\'.");
};

void Environment::assign(Token* name, Token* value)
{
    if(values.find(name->lexeme) != values.end())
    {
        values[name->lexeme] = value;
        return;
    }
    // if the wanted variable cannot be found in the current environment,
    // look up in the enclosing environment
    if(enclosing != nullptr)
    {
        enclosing->assign(name, value);
        return;
    }
    
    throw new RuntimeError(name, "Undefined variable \'" + name->lexeme + "\'.");
}