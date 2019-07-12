#ifndef LOX_NATIVEFUNC
#define LOX_NATIVEFUNC

#include "loxCallable.hpp"
#include "token.hpp"

// LOXNF stands for Lox Native Functions

class LOXNF_Clock : public LoxCallable
{
public:
    virtual size_t arity() override;
    virtual Token* call(Interpreter* _interpreter, std::vector<Token*> _arguments) override;
};

#endif