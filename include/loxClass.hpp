#ifndef LOX_CLASS
#define LOX_CLASS

#include <string>

#include "loxCallable.hpp"
#include "loxFunction.hpp"

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
    std::map<std::string, LoxFunction*> const methods;
    
public:
    LoxClass(std::string _name, std::map<std::string, LoxFunction*> _methods);
    std::string ToString();

    virtual size_t arity() override;
    virtual Token* call(Interpreter* _interpreter, std::vector<Token*> _arguments) override;

    LoxFunction* findMethod(std::string _methodName);
};

#endif