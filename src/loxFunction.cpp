#include "loxFunction.hpp"
#include "environment.hpp"
#include "return.hpp"

LoxFunction::LoxFunction(Function* _declaration, Environment* _closure, bool _isInitializer)
    : declaration(_declaration), closure(_closure), isInitializer(_isInitializer) {}

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
        if(isInitializer) return closure->getAt(0, "this");
        //std::cout << "DEBUG : " << returnException->token->toString() << std::endl;
        return returnException->token;
    }

    if(isInitializer) return closure->getAt(0, "this");

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
    
    return new LoxFunction(declaration, environment, isInitializer);
}