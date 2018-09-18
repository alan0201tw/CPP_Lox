#include "interpreter.hpp"
#include "loxException.hpp"
#include "lox.hpp"

void Interpreter::interpret(Expr* expr)
{
    try
    {
        Token* value = evaluate(expr);
        // in jlox, there is an additional stringify method to converet Object in Java to string.
        // in cpplox, since we represent values with Tokens, we can just call the toString method.
        std::cout << value->toString() << std::endl;
    }
    catch(RuntimeError& error)
    {
        Lox::runtimeError(error);
    }
}

// Visit methods for visiting nodes in AST

Token* Interpreter::visitLiteralExpr(LiteralExpr* expr)
{
    return expr->value;
}

Token* Interpreter::visitGroupingExpr(Grouping* expr)
{
    return evaluate(expr->expression);
}

Token* Interpreter::visitUnaryExpr(Unary* expr)
{
    Token* right = evaluate(expr->right);

    switch(expr->optr->type)
    {
    case TokenType::BANG :
    {
        bool value = !isTruthy(right);
        return boolToken(value);
        break;
    }
    case TokenType::MINUS :
    {
        // detecting runtime errors
        checkNumberOperand(expr->optr, right);

        double value = -(right->literal->doubleValue);
        return doubleToken(value);
        break;
    }
    // there are only one unary optr in lox, which is MINUS, so if we get other types here, we should just break it.
    default:
        break;
    }

    // Unreachable.
    return nullptr;
}

Token* Interpreter::visitBinaryExpr(Binary* expr)
{
    Token* left = evaluate(expr->left);
    Token* right = evaluate(expr->right);

    switch(expr->optr->type)
    {
    case TokenType::MINUS :
        checkNumberOperands(expr->optr, left, right);
        return doubleToken(left->literal->doubleValue - right->literal->doubleValue);
    case TokenType::SLASH :
        checkNumberOperands(expr->optr, left, right);
        return doubleToken(left->literal->doubleValue / right->literal->doubleValue);
    case TokenType::STAR :
        checkNumberOperands(expr->optr, left, right);
        return doubleToken(left->literal->doubleValue * right->literal->doubleValue);
    // for + optr, it is supported for both string and number type. So we need to do a type check here.
    case TokenType::PLUS :
        if(left->type == TokenType::NUMBER && right->type == TokenType::NUMBER)
            return doubleToken(left->literal->doubleValue + right->literal->doubleValue);
        else if(left->type == TokenType::STRING && right->type == TokenType::STRING)
            return stringToken(left->literal->stringValue + right->literal->stringValue);
        // if both cases fail, throw runtime error
        throw new RuntimeError(expr->optr, "Operands must be two numbers or two strings.");

    // Comparison optrs
    case TokenType::GREATER :
        checkNumberOperands(expr->optr, left, right);
        return boolToken(left->literal->doubleValue > right->literal->doubleValue);
    case TokenType::GREATER_EQUAL :
        checkNumberOperands(expr->optr, left, right);
        return boolToken(left->literal->doubleValue >= right->literal->doubleValue);
    case TokenType::LESS :
        checkNumberOperands(expr->optr, left, right);
        return boolToken(left->literal->doubleValue < right->literal->doubleValue);
    case TokenType::LESS_EQUAL :
        checkNumberOperands(expr->optr, left, right);
        return boolToken(left->literal->doubleValue <= right->literal->doubleValue);

    // Equality optrs
    case TokenType::BANG_EQUAL :
        return boolToken(!isEqual(left, right));
    case TokenType::EQUAL_EQUAL :
        return boolToken(isEqual(left, right));

    default:
        break;
    }

    // Unreachable.
    return nullptr;
}

// utility methods
Token* Interpreter::evaluate(Expr* expr)
{
    return expr->accept(this);
}

bool Interpreter::isTruthy(Token* _token)
{
    // false cases : null(cpp), nil(lox), false(lox bool)
    if(_token == nullptr) return false;
    if(_token->type == TokenType::NIL) return false;
    if(_token->type == TokenType::FALSE) return false;

    return true;
}

bool Interpreter::isEqual(Token* a, Token* b)
{
    // nil == nil ( nil is the null value in lox )
    if(a == nullptr && b == nullptr) return true;
    if(a == nullptr) return false;

    // if types are different, return false
    if(a->type != b->type) return false;

    // if types are the same, there are 2 cases where we need to compare the actual value
    // when they're numbers and when they're strings.
    // Another possible value type is bool, but we can differ them with TokenType, so no need to compare value.
    switch(a->type)
    {
    case TokenType::NUMBER :
        return a->literal->doubleValue == b->literal->doubleValue;
    case TokenType::STRING :
        return a->literal->stringValue == b->literal->stringValue;
    default :
        return true;
    }
}

// runtime error detection and checking

void Interpreter::checkNumberOperand(Token* _optr, Token* _operand)
{
    if(_operand->type == TokenType::NUMBER) return;
    throw new RuntimeError(_optr, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token* _optr, Token* _left, Token* _right)
{
    if(_left->type == TokenType::NUMBER && _right->type == TokenType::NUMBER) return;
    throw new RuntimeError(_optr, "Operand must be a numbers.");
}

// my utility methods

Token* Interpreter::boolToken(bool _value)
{
    if(_value == true)
        return new Token(TokenType::TRUE, "true", nullptr, 0);
    return new Token(TokenType::FALSE, "false", nullptr, 0);
}

Token* Interpreter::doubleToken(double _value)
{
    return new Token(TokenType::NUMBER, std::to_string(_value), new Literal(_value), 0);
}

Token* Interpreter::stringToken(std::string _value)
{
    // the lexeme will be "_value"
    return new Token(TokenType::STRING, "\"" + _value + "\"", new Literal(_value), 0);
}