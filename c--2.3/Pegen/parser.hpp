#pragma once

#include "lexer.hpp"
#include "token.hpp"
#include "ast.hpp"
#include <vector>
#include <cstdlib>
using namespace std;

/*
记忆分析结果
*/
struct Memo
{
    int type;   //类型
    int mark;   //标记
    void *node; //节点
};

/*
语法分析
*/
class Parser
{
public:
    Parser(Lexer *);
    TokenInfo *expect(string);              //期望的token字符串
    TokenInfo *expect(TokenType);           //期望的token类型
    void *is_memorized(int, int);           //是否记忆
    void memorize(int, int, void *);        //记忆
    virtual void *start() { return NULL; }; //开始

    Lexer *lexer;
    vector<Memo *> memory;
};

Parser::Parser(Lexer *lexer)
{
    this->lexer = lexer;
}

TokenInfo *Parser::expect(string str)
{
    if (this->lexer->peek_token()->str != str)
        return NULL;
    return this->lexer->next_token();
}

TokenInfo *Parser::expect(TokenType type)
{
    // TODO 改变返回的内容
    if (this->lexer->peek_token()->type != type)
    {
        cout << "expect " << type << " but " << this->lexer->peek_token()->toString() << endl;
        return NULL;
    }
    return this->lexer->next_token();
}

void *Parser::is_memorized(int type, int mark)
{
    for (int i = 0; i < this->memory.size(); i++)
    {
        if (this->memory[i]->type == type && this->memory[i]->mark == mark)
            return this->memory[i]->node;
    }
    return NULL;
}

void Parser::memorize(int type, int mark, void *node)
{
    Memo *memo = new Memo;
    memo->type = type;
    memo->mark = mark;
    memo->node = node;
    this->memory.push_back(memo);
}