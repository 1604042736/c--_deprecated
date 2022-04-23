#pragma once

#include "ast.hpp"

class ASTOpt : public AST
{
public:
    void print(int);
    ASTOpt(AST *);
    AST *node;
};

ASTOpt::ASTOpt(AST *node)
{
    this->node = node;
}

void ASTOpt::print(int space)
{
    this->print_space(space);
    cout << "Opt" << endl;
    this->print_space(space + 1);
    cout << "node" << endl;
    this->node->print(space + 2);
}