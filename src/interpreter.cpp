#include "interpreter.hpp"

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
        if(value == true)
            return new Token(TokenType::TRUE, "", nullptr, 0);
        return new Token(TokenType::FALSE, "", nullptr, 0);
        break;
    }
    case TokenType::MINUS :
    {
        double value = -(right->literal->doubleValue);
        return new Token(TokenType::NUMBER, "", new Literal(value), 0);
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
        return new Token(TokenType::NUMBER, "", new Literal(left->literal->doubleValue - right->literal->doubleValue), 0);
    case TokenType::SLASH :
        return new Token(TokenType::NUMBER, "", new Literal(left->literal->doubleValue / right->literal->doubleValue), 0);
    case TokenType::STAR :
        return new Token(TokenType::NUMBER, "", new Literal(left->literal->doubleValue * right->literal->doubleValue), 0);
    // for + optr, it is supported for both string and number type. So we need to do a type check here.
    case TokenType::PLUS :
        if(left->type == TokenType::NUMBER && right->type == TokenType::NUMBER)
            return new Token(TokenType::NUMBER, "", new Literal(left->literal->doubleValue + right->literal->doubleValue), 0);
        else if(left->type == TokenType::STRING && right->type == TokenType::STRING)
            return new Token(TokenType::NUMBER, "", new Literal(left->literal->stringValue + right->literal->stringValue), 0);

    // Comparison optrs
    case TokenType::GREATER:
        if(left->literal->doubleValue > right->literal->doubleValue)
            return new Token(TokenType::TRUE, "", nullptr, 0);
        return new Token(TokenType::FALSE, "", nullptr, 0);
    case TokenType::GREATER_EQUAL:
        if(left->literal->doubleValue >= right->literal->doubleValue)
            return new Token(TokenType::TRUE, "", nullptr, 0);
        return new Token(TokenType::FALSE, "", nullptr, 0);
    case TokenType::LESS:
        if(left->literal->doubleValue < right->literal->doubleValue)
            return new Token(TokenType::TRUE, "", nullptr, 0);
        return new Token(TokenType::FALSE, "", nullptr, 0);
    case TokenType::LESS_EQUAL:
        if(left->literal->doubleValue <= right->literal->doubleValue)
            return new Token(TokenType::TRUE, "", nullptr, 0);
        return new Token(TokenType::FALSE, "", nullptr, 0);

    // Equality optrs
    // TODO : implement

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