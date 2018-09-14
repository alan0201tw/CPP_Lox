#include "loxException.hpp"

char const* ParseError::what() const noexcept
{
    return "Parse Error";
}