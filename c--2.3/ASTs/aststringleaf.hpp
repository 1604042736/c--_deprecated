#pragma once

#include "ast.hpp"

class ASTStringLeaf : public AST
{
public:
    void print(int);
    ASTStringLeaf(string);
    string value;
    void gen();
};

ASTStringLeaf::ASTStringLeaf(string value)
{
    this->value = value;
}

void ASTStringLeaf::print(int space)
{
    this->print_space(space);
    cout << "StringLeaf " << this->value << endl;
}

void ASTStringLeaf::gen()
{
    pegen->out << "this->expect(" + this->value + ")" << endl;
}