#pragma once

#include "ast.hpp"
#include "pegen.hpp"

class ASTNamedItem : public AST
{
public:
    void print(int);
    ASTNamedItem(string, AST *, string);
    string name;
    AST *item;
    string type;
    void gen();
};

ASTNamedItem::ASTNamedItem(string name, AST *item, string type)
{
    this->name = name;
    this->item = item;
    this->type = type;
}

void ASTNamedItem::print(int space)
{
    this->print_space(space);
    cout << "NamedItem type " << this->type << " used " << this->name << endl;
    this->print_space(space + 1);
    cout << "item" << endl;
    this->item->print(space + 2);
}

void ASTNamedItem::gen()
{
    if (this->name != "")
    {
        pegen->defines.push_back(this->item->get_type() + " " + this->name);
        pegen->out << this->name << "=";
        pegen->assign = false;
        this->item->gen();
        pegen->assign = true;
    }
}