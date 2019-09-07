#ifndef LOX_FUNCTION
#define LOX_FUNCTION

#include "interpreter.hpp"
#include "loxCallable.hpp"

class LoxInstance;

class LoxFunction : public LoxCallable
{
public:
    Function* declaration;
    Environment* closure;
    bool isInitializer;

public:
    LoxFunction(Function* _declaration, Environment* _closure, bool _isInitializer = false);
    virtual Token* call(Interpreter* _interpreter, std::vector<Token*> _arguments) override;
    virtual size_t arity() override;

public:
    LoxFunction* bind(LoxInstance* _instance);
};

#endif
