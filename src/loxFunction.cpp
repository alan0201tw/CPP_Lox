#include "loxFunction.hpp"
#include "environment.hpp"

LoxFunction::LoxFunction(Function* _declaration)
{
    declaration = _declaration;
}

Token* LoxFunction::call(Interpreter* _interpreter, std::vector<Token*> _arguments)
{
    Environment* environment = new Environment(_interpreter->globals);
    for(uint i = 0; i < declaration->params.size(); i++)
    {
        environment->define(declaration->params[i]->lexeme, _arguments[i]);
    }

    _interpreter->executeBlock(declaration->body, environment);

    return nullptr;
}

size_t LoxFunction::arity()
{
    return declaration->params.size();
}