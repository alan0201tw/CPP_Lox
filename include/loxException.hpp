#ifndef LOX_EXCEPTION
#define LOX_EXCEPTION

#include <exception>

class ParseError : public std::exception
{
public:
    // inherited method from std::exception, should return a string indicating the exception(error)
    virtual char const* what() const noexcept override;
};

#endif