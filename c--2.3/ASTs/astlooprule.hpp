#pragma once

#include "ast.hpp"
#include "pegen.hpp"

class ASTLoopRule : public AST
{
public:
    ASTLoopRule(string, string, string, AST *);
    AST *node;
    string name;
    string return_type;
    string base_type;
    void gen();
};

ASTLoopRule::ASTLoopRule(string name, string return_type, string base_type, AST *node)
{
    this->node = node;
    this->name = name;
    this->base_type = base_type;
    this->return_type = return_type;
}

void ASTLoopRule::gen()
{
    pegen->out << this->return_type + " _res_list;" << endl;
    pegen->out << this->base_type + " _res=NULL;" << endl;
    pegen->out << "if((vector<void*>*)this->is_memorized(" << this->name << "_type,this->lexer->mark"
               << ")!=NULL)" << endl;
    pegen->out << "return *(vector<void*>*)this->is_memorized(" << this->name << "_type,this->lexer->mark"
               << ");" << endl;
    pegen->out << "while(true)" << endl;
    pegen->out << "{" << endl;
    this->node->gen();
    pegen->out << "break;" << endl;
    pegen->out << "done:" << endl;
    pegen->out << "_res_list.push_back(_res);" << endl;
    pegen->out << "}";
    pegen->out << "real_done:" << endl;
    pegen->out << "this->memorize(" << this->name << "_type,this->lexer->mark,&_res_list);" << endl;
    pegen->out << "return _res_list;" << endl;
}