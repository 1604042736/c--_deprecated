#pragma once

#include "ast.hpp"

class ASTRepeat1 : public AST
{
public:
    void print(int);
    ASTRepeat1(AST *);
    AST *node;
};

ASTRepeat1::ASTRepeat1(AST *node)
{
    this->node = node;
}

void ASTRepeat1::print(int space)
{
    this->print_space(space);
    cout << "Repeat1" << endl;
    this->print_space(space + 1);
    cout << "node" << endl;
    this->node->print(space + 2);
}