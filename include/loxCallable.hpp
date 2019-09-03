#ifndef LOX_CALLABLE
#define LOX_CALLABLE

// this is actually an interface.
// in cpp, interface can be seen as an virtual class,
// with only pure virtual methods and no members.

#include <vector>

class Token;
class Interpreter;

class LoxCallable
{
public:
    virtual size_t arity() = 0;
    virtual Token* call(Interpreter* _interpreter, std::vector<Token*> _arguments) = 0;
};

#endif