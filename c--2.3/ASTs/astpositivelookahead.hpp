#pragma once

#include "ast.hpp"

class ASTPositiveLookahead : public AST
{
public:
    void print(int);
    ASTPositiveLookahead(AST *);
    AST *node;
    string sign = "&";
};

ASTPositiveLookahead::ASTPositiveLookahead(AST *node)
{
    this->node = node;
}

void ASTPositiveLookahead::print(int space)
{
    this->print_space(space);
    cout << "PositiveLookahead" << endl;
    this->print_space(space + 1);
    cout << "node" << endl;
    this->node->print(space + 2);
}