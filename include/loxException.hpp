#ifndef LOX_EXCEPTION
#define LOX_EXCEPTION

#include <exception>

#include "token.hpp"

class ParseError : public std::exception
{
public:
    // inherited method from std::exception, should return a string indicating the exception(error)
    virtual char const* what() const noexcept override;
};

// runtime_error inherits std::exception
class RuntimeError : public std::runtime_error
{
public:
    Token* token;

    RuntimeError(Token* _token, std::string _message) : runtime_error(_message)
    {
        token = _token;
    }

    // inherited method from std::exception, should return a string indicating the exception(error)
    virtual char const* what() const noexcept override;
};

#endif