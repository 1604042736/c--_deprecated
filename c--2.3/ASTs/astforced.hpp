#pragma once

#include "ast.hpp"

class ASTForced : public AST
{
public:
    void print(int space);
    ASTForced(AST *);
    AST *node;
};

ASTForced::ASTForced(AST *node)
{
    this->node = node;
}

void ASTForced::print(int space)
{
    this->print_space(space);
    cout << "Forced" << endl;
    this->print_space(space + 1);
    cout << "node" << endl;
    this->node->print(space + 2);
}