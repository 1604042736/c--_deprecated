#pragma once

#include "ast.hpp"

class ASTNegativeLookahead : public AST
{
public:
    void print(int);
    ASTNegativeLookahead(AST *);
    AST *node;
    string sign = "!";
};

ASTNegativeLookahead::ASTNegativeLookahead(AST *node)
{
    this->node = node;
}

void ASTNegativeLookahead::print(int space)
{
    this->print_space(space);
    cout << "NegativeLookahead" << endl;
    this->print_space(space + 1);
    cout << "node" << endl;
    this->node->print(space + 2);
}