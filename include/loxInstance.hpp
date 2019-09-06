#ifndef LOX_INSTANCE
#define LOX_INSTANCE

#include "loxClass.hpp"

#include "token.hpp"

#include <map>

class LoxInstance
{
private:
    LoxClass* klass;
    std::map<std::string, Token*> fields;

public:
    LoxInstance(LoxClass* _klass);

    Token* get(Token* name);
    void set(Token* name, Token* value);

    std::string ToString();
};

#endif