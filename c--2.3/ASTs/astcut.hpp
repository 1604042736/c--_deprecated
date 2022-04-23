#pragma once

#include "ast.hpp"

class ASTCut : public AST
{
public:
    void print(int space);
};

void ASTCut::print(int space)
{
    this->print_space(space);
    cout << "Cut" << endl;
}