#include "environment.hpp"

#include "loxException.hpp"

void Environment::define(std::string name, Token* value)
{
    // this is 0, this is null
    //std::cout << this << std::endl;

    delete(values[name]);
    //std::cout << value->toString() << std::endl;
    values[name] = value;
};

Token* Environment::get(Token* name)
{
    if(values.find(name->lexeme) != values.end())
    {
        return values[name->lexeme];
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
    
    throw new RuntimeError(name, "Undefined variable \'" + name->lexeme + "\'.");
}