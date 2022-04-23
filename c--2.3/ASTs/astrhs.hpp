#pragma once

#include "ast.hpp"

/*
语法规则右部
*/
class ASTRhs : public AST
{
public:
    void print(int);
    ASTRhs(vector<AST *>);
    vector<AST *> items; //右部项
    void gen();
};

ASTRhs::ASTRhs(vector<AST *> items)
{
    this->items = items;
}

void ASTRhs::print(int space)
{
    this->print_space(space);
    cout << "Rhs" << endl;
    this->print_space(space + 1);
    cout << "items" << endl;
    for (int i = 0; i < this->items.size(); i++)
    {
        this->items[i]->print(space + 2);
    }
}

void ASTRhs::gen()
{
    
}