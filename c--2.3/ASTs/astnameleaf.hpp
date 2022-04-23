#pragma once

#include "ast.hpp"

class ASTNameLeaf : public AST
{
public:
    void print(int);
    ASTNameLeaf(string);
    string value;
    void gen();
    string toString();
    string get_type();
};

ASTNameLeaf::ASTNameLeaf(string value)
{
    this->value = value;
}

void ASTNameLeaf::print(int space)
{
    this->print_space(space);
    cout << "NameLeaf " << this->value << endl;
}

string ASTNameLeaf::toString()
{
    return this->value;
}

void ASTNameLeaf::gen()
{
    if (pegen->assign)
    {
        if (this->value == "NAME")
        {
            pegen->defines.push_back("TokenInfo* name");
            pegen->out << "name=this->expect(TK_ID)" << endl;
        }
        else if (this->value == "STRING")
        {
            pegen->defines.push_back("TokenInfo* str");
            pegen->out << "str=this->expect(TK_STRING)" << endl;
        }
        else if (this->value == "NUMBER")
        {
            pegen->defines.push_back("TokenInfo* number");
            pegen->out << "number=this->expect(TK_NUM)" << endl;
        }
        else
        {
            ASTRule *rule = (ASTRule *)pegen->rules_def[this->value];
            pegen->defines.push_back(rule->return_type + " _" + rule->name);
            pegen->out << "_" + this->value + "=this->" + this->value + "()" << endl;
        }
    }
    else
    {
        if (this->value == "NAME")
        {
            pegen->out << "this->expect(TK_ID)" << endl;
        }
        else if (this->value == "STRING")
        {
            pegen->out << "this->expect(TK_STRING)" << endl;
        }
        else if (this->value == "NUMBER")
        {
            pegen->out << "this->expect(TK_NUM)" << endl;
        }
        else
        {
            pegen->out << "this->" + this->value + "() " << endl;
        }
    }
}

string ASTNameLeaf::get_type()
{
    ASTRule *rule = (ASTRule *)pegen->rules_def[this->value];
    return rule->return_type;
}