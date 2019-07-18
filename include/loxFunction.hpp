#ifndef LOX_FUNCTION
#define LOX_FUNCTION

#include "interpreter.hpp"

class LoxFunction : public LoxCallable
{
public:
    LoxFunction(Function* _declaration);
    virtual Token* call(Interpreter* _interpreter, std::vector<Token*> _arguments) override;
    virtual size_t arity() override;

    Function* declaration;
};

#endif
