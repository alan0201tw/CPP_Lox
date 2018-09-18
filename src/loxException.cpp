#include "loxException.hpp"

char const* ParseError::what() const noexcept
{
    return "Parse Error";
}

char const* RuntimeError::what() const noexcept
{
    return std::runtime_error::what();
}