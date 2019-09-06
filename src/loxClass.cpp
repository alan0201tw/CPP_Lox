#include "loxClass.hpp"

#include "loxInstance.hpp"
#include "token.hpp"

LoxClass::LoxClass(std::string _name) : name(_name) {}

std::string LoxClass::ToString()
{
    return name;
}

size_t LoxClass::arity()
{
    return 0;
}

Token* LoxClass::call(Interpreter* _interpreter, std::vector<Token*> _arguments)
{
    LoxInstance* instance = new LoxInstance(this);
    return new Token(TokenType::INSTANCE, "Lox Instance", new Literal(instance), 0);
}
