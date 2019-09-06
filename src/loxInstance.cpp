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

std::string LoxInstance::ToString()
{
    return klass->name + " instance";
}
