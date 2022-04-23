#pragma once

#include "ast.hpp"
#include "pegen.hpp"
#include "astlooprule.hpp"

class ASTRepeat0 : public AST
{
public:
    void print(int);
    ASTRepeat0(AST *);
    AST *node;
    void gen();
    string get_type();
};

ASTRepeat0::ASTRepeat0(AST *node)
{
    this->node = node;
}

void ASTRepeat0::print(int space)
{
    this->print_space(space);
    cout << "Repeat0" << endl;
    this->print_space(space + 1);
    cout << "node" << endl;
    this->node->print(space + 2);
}

void ASTRepeat0::gen()
{
    string type = "_loop" + to_string(pegen->loop_count++) + "_rule";
    pegen->out << type + "()" << endl;
    pegen->type_def[type + "_type"] = pegen->type_count++;
    pegen->loop_def[type] = new ASTLoopRule(type, "vector<void*>", "void*", this->node);
}

string ASTRepeat0::get_type()
{
    return "vector<void*>";
}