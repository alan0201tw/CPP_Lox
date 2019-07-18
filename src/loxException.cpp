#include "loxException.hpp"
#include <stdexcept>

char const* ParseError::what() const noexcept
{
    return "Parse Error";
}

char const* RuntimeError::what() const noexcept
{
    return std::runtime_error::what();
}