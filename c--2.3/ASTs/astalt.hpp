#pragma once

#include "astrule.hpp"
#include "ast.hpp"

class ASTAlt : public AST
{
public:
    void print(int space);
    ASTAlt(vector<AST *>, string);
    vector<AST *> items; //右部项
    string action;       //动作
    void gen();
    string get_type();
};

ASTAlt::ASTAlt(vector<AST *> items, string action)
{
    this->items = items;
    this->action = action;
}

void ASTAlt::print(int space = 0)
{
    this->print_space(space);
    cout << "Alt" << endl;
    this->print_space(space + 1);
    cout << "items" << endl;
    for (int i = 0; i < this->items.size(); i++)
    {
        this->items[i]->print(space + 2);
    }
}

void ASTAlt::gen()
{
    auto pos = pegen->out.tellp();
    int def_pos = pegen->defines.size();
    for (int i = 0; i < this->items.size() * 10; i++)
    {
        for (int j = 0; j < 10; j++)
            pegen->out << " ";
    }
    pegen->out << endl;

    pegen->out << "if(" << endl;
    for (int i = 0; i < this->items.size(); i++)
    {
        pegen->out << "(";
        this->items[i]->gen();
        pegen->out << ")";
        if (i < this->items.size() - 1)
            pegen->out << "&&" << endl;
    }
    pegen->out << ")\n{" << endl;
    if (this->action != "")
    {
        pegen->out << "_res=" << this->action << ";" << endl;
    }
    pegen->out << "goto done;" << endl;
    pegen->out << "}" << endl;
    auto _pos = pegen->out.tellp();

    pegen->out.seekp(pos);
    for (int i = pegen->defines.size() - 1; i >= def_pos; i--)
    {
        pegen->out << pegen->defines[i] << ";" << endl;
        pegen->defines.pop_back();
    }
    pegen->out.seekp(_pos);
}

string ASTAlt::get_type()
{
    return this->items[0]->get_type();
}