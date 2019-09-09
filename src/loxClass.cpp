#include "loxClass.hpp"

#include "loxInstance.hpp"
#include "token.hpp"

LoxClass::LoxClass(std::string _name, LoxClass* _superclass, std::map<std::string, LoxFunction*> _methods)
    : name(_name), superclass(_superclass), methods(_methods) {}

std::string LoxClass::ToString()
{
    return name;
}

size_t LoxClass::arity()
{
    LoxFunction* initializer = findMethod("init");
    if(initializer == nullptr) return 0;

    return initializer->arity();
}

Token* LoxClass::call(Interpreter* _interpreter, std::vector<Token*> _arguments)
{
    LoxInstance* instance = new LoxInstance(this);
    // find the initializer "init", and call it
    LoxFunction* initializer = findMethod("init");
    if(initializer != nullptr)
    {
        initializer->bind(instance)->call(_interpreter, _arguments);
    }

    return new Token(TokenType::INSTANCE, "Lox Instance", new Literal(instance), 0);
}

// will return nullptr if method is not found
LoxFunction* LoxClass::findMethod(std::string _methodName)
{
    if(methods.find(_methodName) != methods.end())
    {
        return methods.at(_methodName);
    }

    if (superclass != nullptr)
    {
        return superclass->findMethod(_methodName);
    }
    
    return nullptr;
}