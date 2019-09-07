#include "loxFunction.hpp"
#include "environment.hpp"
#include "return.hpp"

LoxFunction::LoxFunction(Function* _declaration, Environment* _closure)
{
    declaration = _declaration;
    closure = _closure;
}

Token* LoxFunction::call(Interpreter* _interpreter, std::vector<Token*> _arguments)
{
    // Environment* environment = new Environment(_interpreter->globals);
    Environment* environment = new Environment(this->closure);
    for(uint i = 0; i < declaration->params.size(); i++)
    {
        environment->define(declaration->params[i]->lexeme, _arguments[i]);
    }

    //_interpreter->executeBlock(declaration->body, environment);

    // handling return statements
    try
    {
        _interpreter->executeBlock(declaration->body, environment);
    }
    catch(ReturnExcept* returnException)
    {
        //std::cout << "DEBUG : " << returnException->token->toString() << std::endl;
        return returnException->token;
    }

    return nullptr;
}

size_t LoxFunction::arity()
{
    return declaration->params.size();
}

LoxFunction* LoxFunction::bind(LoxInstance* _instance)
{
    Environment* environment = new Environment(closure);
    environment->define("this", 
        new Token(TokenType::INSTANCE, "Lox Instance", new Literal(_instance), 0));
    
    return new LoxFunction(declaration, environment);
}