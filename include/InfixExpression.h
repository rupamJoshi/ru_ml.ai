#pragma once
class NDArray;
template<typename TT1, typename TT2>
class InfixExpression
{
public:
    TT1 *left;
    TT2 *right;
    char oper;
    InfixExpression(TT1 *left, char oper, TT2 *right)
    {
        this->left = left;
        this->right = right;
        this->oper = oper;
    }
};
