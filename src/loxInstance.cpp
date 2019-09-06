#include "loxInstance.hpp"

#include "loxException.hpp"

LoxInstance::LoxInstance(LoxClass* _klass) : klass(_klass) {}

Token* LoxInstance::get(Token* name)
{
    if(fields.find(name->lexeme) != fields.end())
    {
        return fields[name->lexeme];
    }

    throw new RuntimeError(name, "Undefined property '" + name->lexeme + "'.");
}

void LoxInstance::set(Token* name, Token* value)
{
    fields[name->lexeme] = value;
}

std::string LoxInstance::ToString()
{
    return klass->name + " instance";
}
