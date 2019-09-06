#ifndef LOX_CLASS
#define LOX_CLASS

#include <string>

#include "loxCallable.hpp"

/*
    In Lox, we use syntaxes like...

    obj = SomeClass();

    to create instances. Making a class in Lox very much behave
    like a callable function that acts like a factory which produces
    instances of that class.

    So we'll take advantage of the previously made LoxCallable for this.
*/
class LoxClass : public LoxCallable
{
public:
    std::string const name;
    
public:
    LoxClass(std::string _name);
    std::string ToString();

    virtual size_t arity() override;
    virtual Token* call(Interpreter* _interpreter, std::vector<Token*> _arguments) override;
};

#endif