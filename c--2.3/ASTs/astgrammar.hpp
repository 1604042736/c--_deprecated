#pragma once

#include "ast.hpp"
#include "pegen.hpp"
#include "astlooprule.hpp"
#include "astrule.hpp"

/*
多个语法的语法树
*/
class ASTGrammar : public AST
{
public:
    void print(int);
    ASTGrammar(vector<AST *>, string);
    ASTGrammar(vector<AST *>);
    vector<AST *> rules;
    string metas;
    void gen();
};

ASTGrammar::ASTGrammar(vector<AST *> rules, string metas)
{
    this->rules = rules;
    this->metas = metas;
}

ASTGrammar::ASTGrammar(vector<AST *> rules)
{
    this->rules = rules;
}

void ASTGrammar::print(int space)
{
    this->print_space(space);
    cout << "Grammar" << endl;
    this->print_space(space + 1);
    cout << "rules" << endl;
    for (int i = 0; i < this->rules.size(); i++)
    {
        this->rules[i]->print(space + 2);
    }
}

void ASTGrammar::gen()
{
    pegen->out << "#pragma once" << endl;
    pegen->out << "#include \"parser.hpp\"" << endl;
    pegen->out << "#include \"token.hpp\"" << endl;

    pegen->out << this->metas.substr(1, this->metas.length() - 2) << endl;

    for (int i = 0; i < this->rules.size(); i++)
    {
        pegen->type_def[((ASTRule *)this->rules[i])->name + "_type "] = pegen->type_count++;
    }

    for (int i = 0; i < this->rules.size(); i++)
    {
        ASTRule *rule = (ASTRule *)this->rules[i];
        string define = rule->get_define();
        pegen->rules_def[rule->name] = rule;
    }
    auto pos = pegen->out.tellp();

    for (int i = 0; i < (pegen->rules_def.size() + pegen->type_def.size()) * 4; i++)
    {
        for (int j = 0; j < 20; j++)
            pegen->out << " ";
    }
    pegen->out << endl;

    for (auto it = pegen->rules_def.begin(); it != pegen->rules_def.end(); it++)
    {
        ASTRule *rule = (ASTRule *)it->second;
        pegen->out << rule->return_type + " " + pegen->parsername + "::" + rule->name + "()" << endl;
        pegen->out << "{" << endl;
        rule->gen();
        pegen->out << "}" << endl;
    }

    for (auto it = pegen->loop_def.begin(); it != pegen->loop_def.end(); it++)
    {
        ASTLoopRule *rule = (ASTLoopRule *)it->second;
        pegen->out << rule->return_type + " " + pegen->parsername + "::" + rule->name + "()" << endl;
        pegen->out << "{" << endl;
        rule->gen();
        pegen->out << "}" << endl;
    }
    auto _pos = pegen->out.tellp();
    pegen->out.seekp(pos);

    for (auto it = pegen->type_def.begin(); it != pegen->type_def.end(); it++)
    {
        pegen->out << "#define " << it->first << " " << it->second << endl;
    }
    pegen->out << "class " << pegen->parsername << ":public Parser" << endl;
    pegen->out << "{" << endl;
    pegen->out << "public:" << endl;
    pegen->out << pegen->parsername + "(Lexer *lexer) : Parser(lexer) {}" << endl;
    for (auto it = pegen->rules_def.begin(); it != pegen->rules_def.end(); it++)
    {
        pegen->out << ((ASTRule *)it->second)->get_define() << ";" << endl;
    }
    for (auto it = pegen->loop_def.begin(); it != pegen->loop_def.end(); it++)
    {
        pegen->out << ((ASTLoopRule *)it->second)->return_type << " " << it->first << "();" << endl;
    }
    pegen->out << "};" << endl;
    pegen->out.seekp(_pos);
}