#pragma once

#include "ast.hpp"

#include "pegen.hpp"

/*
一个语法规则
*/
class ASTRule : public AST
{
public:
    void print(int);
    ASTRule(string, string, vector<AST *>);
    string name;         //名称
    string return_type;  //返回类型
    vector<AST *> rhs;   //右部
    string get_define(); //获取声明
    void gen();
};

ASTRule::ASTRule(string name, string return_type, vector<AST *> rhs)
{
    this->name = name;
    this->return_type = return_type;
    this->rhs = rhs;
}

void ASTRule::print(int space)
{
    this->print_space(space);
    cout << "Rule name " << this->name << " return " << this->return_type << endl;
    this->print_space(space + 1);
    cout << "rhs" << endl;
    for (int i = 0; i < this->rhs.size(); i++)
    {
        this->rhs[i]->print(space + 2);
    }
}

string ASTRule::get_define()
{
    string result = this->return_type + " " + this->name + "()";
    return result;
}

void ASTRule::gen()
{
    pegen->out << this->return_type + " _res=NULL;" << endl;
    pegen->out << "if(_res=(" + this->return_type + ")this->is_memorized(" << this->name << "_type,this->lexer->mark"
               << "))" << endl;
    pegen->out << "return _res;" << endl;
    pegen->out << "int mark=this->lexer->mark;TokenInfo* token=this->lexer->peek_token();" << endl;
    for (int i = 0; i < this->rhs.size(); i++)
    {
        this->rhs[i]->gen();
        pegen->out << "this->lexer->mark=mark;this->lexer->token=token;" << endl;
    }
    pegen->out << "done:" << endl;
    pegen->out << "this->memorize(" << this->name << "_type,this->lexer->mark,_res);" << endl;
    pegen->out << "return _res;" << endl;
}