#pragma once

#include "ast.hpp"

class ASTGroup : public AST
{
public:
    void print(int);
    ASTGroup(vector<AST *>);
    vector<AST *> rhs;
    void gen();
    string get_type();
};

ASTGroup::ASTGroup(vector<AST *> rhs)
{
    this->rhs = rhs;
}

void ASTGroup::print(int space)
{
    this->print_space(space);
    cout << "Group" << endl;
    this->print_space(space + 1);
    cout << "rhs" << endl;
    for (int i = 0; i < this->rhs.size(); i++)
    {
        this->rhs[i]->print(space + 2);
    }
}

void ASTGroup::gen()
{
    pegen->out << "void* _res=NULL;" << endl;
    pegen->out << "int mark=this->lexer->mark;TokenInfo* token=this->lexer->peek_token();" << endl;
    for (int i = 0; i < this->rhs.size(); i++)
    {
        this->rhs[i]->gen();
        pegen->out << "this->lexer->mark=mark;this->lexer->token=token;" << endl;
    }
}

string ASTGroup::get_type()
{
    return this->rhs[0]->get_type();
}