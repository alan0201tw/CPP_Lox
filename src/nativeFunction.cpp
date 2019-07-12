#include <ctime>

#include "nativeFunction.hpp"

size_t LOXNF_Clock::arity()
{
    return 0;
}

Token* LOXNF_Clock::call(Interpreter* _interpreter, std::vector<Token*> _arguments)
{
    double value = (double)std::time(0);

    return new Token(TokenType::NUMBER, std::to_string(value), new Literal(value), 0);
}