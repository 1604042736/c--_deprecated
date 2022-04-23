#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
#include "token.hpp"
using namespace std;

#define isId(n) (('A' <= n && n <= 'Z') || ('a' <= n && n <= 'z') || n == '_')
#define isWhite(n) (n == ' ' || n == '\t' || n == '\n')
#define isNum(n) (('0' <= n && n <= '9'))

enum State //状态
{
    S_START,  //开始
    S_ID,     //标识符
    S_NUM,    //数字(整数)
    S_FLOAT,  //数字(小数)
    S_STRING, //字符串
    S_CHAR,   //字符
    S_GT,     // gt与geq的判断
    S_LT,
    S_EQ,
    S_NOT,
};

/*
词法分析
*/
class Lexer
{
public:
    Lexer(string);
    TokenInfo *next_token(); //获取下一个token
    TokenInfo *peek_token(); //获取当前token
    char getch();            //获取当前字符
    void ungetch();          //返回上一个字符
    char escape_char(char);  //转义字符

    string filename;         //文件名
    TokenInfo *token;        //当前token
    string code;             //代码
    int mark = 0;            //当前位置
    int line = 1, colum = 1; //当前行和列
    bool eof = false;        //是否到达文件结尾
};

Lexer::Lexer(string filename) : filename(filename)
{
    /*读取全部代码*/
    ifstream t(filename);
    stringstream buffer;
    buffer << t.rdbuf();
    this->code = buffer.str();
    t.close();

    this->token = this->next_token();
}

TokenInfo *Lexer::next_token()
{
    char ch = this->getch();
    while (isWhite(ch)) //跳过空白字符
    {
        ch = this->getch();
    }

    TokenType tokentype;
    string str;        //存储token对应的字符串
    bool flag = false; //是否将当前字符存入str中
    bool end = false;  //是否结束
    State state = S_START;

    while (true)
    {
        flag = false;
        switch (state)
        {
        case S_START:
            if (isId(ch))
            {
                state = S_ID;
                flag = true;
                tokentype = TK_ID;
            }
            else if (isNum(ch))
            {
                state = S_NUM;
                flag = true;
                tokentype = TK_NUM;
            }
            else if (ch == '"')
            {
                state = S_STRING;
                flag = true;
                tokentype = TK_STRING;
            }
            else if (ch == '\'')
            {
                state = S_CHAR;
                flag = true;
                tokentype = TK_CHAR;
            }
            else if (ch == '>')
            {
                state = S_GT;
                flag = true;
            }
            else if (ch == '<')
            {
                state = S_LT;
                flag = true;
            }
            else if (ch == '=')
            {
                state = S_EQ;
                flag = true;
            }
            else if (ch == '!')
            {
                state = S_NOT;
                flag = true;
            }
            else if (ch == '+')
            {
                flag = true;
                tokentype = TK_ADD;
                end = true;
            }
            else if (ch == '-')
            {
                flag = true;
                tokentype = TK_SUB;
                end = true;
            }
            else if (ch == '*')
            {
                flag = true;
                tokentype = TK_STAR;
                end = true;
            }
            else if (ch == '/')
            {
                flag = true;
                tokentype = TK_DIV;
                end = true;
            }
            else if (ch == '%')
            {
                flag = true;
                tokentype = TK_MOD;
                end = true;
            }
            else if (ch == '(')
            {
                flag = true;
                tokentype = TK_LLITTLE;
                end = true;
            }
            else if (ch == ')')
            {
                flag = true;
                tokentype = TK_RLITTLE;
                end = true;
            }
            else if (ch == '[')
            {
                flag = true;
                tokentype = TK_LMIDDLE;
                end = true;
            }
            else if (ch == ']')
            {
                flag = true;
                tokentype = TK_RMIDDLE;
                end = true;
            }
            else if (ch == '{')
            {
                flag = true;
                tokentype = TK_LLARGE;
                end = true;
            }
            else if (ch == '}')
            {
                flag = true;
                tokentype = TK_RLARGE;
                end = true;
            }
            else if (ch == '.')
            {
                flag = true;
                tokentype = TK_POINT;
                end = true;
            }
            else if (ch == ',')
            {
                flag = true;
                tokentype = TK_COMMA;
                end = true;
            }
            else if (ch == ';')
            {
                flag = true;
                tokentype = TK_SEMI;
                end = true;
            }
            else if (ch == '#')
            {
                flag = true;
                tokentype = TK_WELL;
                end = true;
            }
            else if (ch == ':')
            {
                flag = true;
                tokentype = TK_COLON;
                end = true;
            }
            else if (ch == '|')
            {
                flag = true;
                tokentype = TK_OR;
                end = true;
            }
            else if (ch == '&')
            {
                flag = true;
                tokentype = TK_AND;
                end = true;
            }
            else if (ch == '?')
            {
                flag = true;
                tokentype = TK_QUES;
                end = true;
            }
            else if (ch == '~')
            {
                flag = true;
                tokentype = TK_LDI;
                end = true;
            }
            else if (ch == '@')
            {
                flag = true;
                tokentype = TK_AT;
                end = true;
            }
            else if (ch == EOF)
            {
                end = true;
                tokentype = TK_FILEEND;
            }
            break;
        case S_ID:
            flag = true;
            if (!isId(ch))
            {
                this->ungetch();
                flag = false;
                end = true;
            }
            break;
        case S_NUM:
            flag = true;
            if (ch == '.')
            {
                state = S_FLOAT;
            }
            else if (!isNum(ch))
            {
                this->ungetch();
                flag = false;
                end = true;
            }
            break;
        case S_FLOAT:
            flag = true;
            if (!isNum(ch))
            {
                this->ungetch();
                flag = false;
                end = true;
            }
            break;
        case S_STRING:
            flag = true;
            if (ch == '"')
            {
                end = true;
            }
            break;
        case S_CHAR:
            flag = true;
            if (ch == '\'')
            {
                end = true;
            }
            break;
        case S_GT:
            if (ch == '=')
            {
                flag = true;
                end = true;
                tokentype = TK_GEQ;
            }
            else
            {
                this->ungetch();
                end = true;
                tokentype = TK_GT;
            }
            break;
        case S_LT:
            if (ch == '=')
            {
                flag = true;
                end = true;
                tokentype = TK_LEQ;
            }
            else
            {
                this->ungetch();
                end = true;
                tokentype = TK_LT;
            }
            break;
        case S_EQ:
            if (ch == '=')
            {
                flag = true;
                end = true;
                tokentype = TK_EQ;
            }
            else
            {
                this->ungetch();
                end = true;
                tokentype = TK_ASSIGN;
            }
            break;
        case S_NOT:
            if (ch == '=')
            {
                flag = true;
                end = true;
                tokentype = TK_NEQ;
            }
            else
            {
                this->ungetch();
                end = true;
                tokentype = TK_NOT;
            }
            break;
        }
        if (flag)
            str += ch;
        if (end)
            break;
        ch = this->getch();
    }
    this->token = new TokenInfo(tokentype, this->line, this->colum, str);
    return this->token;
}

TokenInfo *Lexer::peek_token()
{
    return this->token;
}

char Lexer::getch()
{
    if (this->mark >= this->code.length())
    {
        this->eof = true;
        return EOF;
    }
    if (this->code[this->mark] == '\n')
    {
        this->line++;
        this->colum = 0;
    }
    this->colum++;
    return this->code[this->mark++];
}

void Lexer::ungetch()
{
    if (!this->eof && this->mark > 0)
        this->mark--;
}

char Lexer::escape_char(char ch)
{
    switch (ch)
    {
    case 'n':
        return '\n';
    case 'r':
        return '\r';
    case 't':
        return '\t';
    }
}