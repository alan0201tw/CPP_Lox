#ifndef LOX_RETURNEXCEPT
#define LOX_RETURNEXCEPT

#include <stdexcept>
#include "token.hpp"

class ReturnExcept : public std::runtime_error
{
public:
    Token* const token;

    ReturnExcept(Token* _token) : runtime_error(""), token(_token) {}
};

class BreakExcept : public std::runtime_error
{
public:
    BreakExcept() : runtime_error("") {}
};


#endif