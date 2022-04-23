#pragma once

#include <string>
#include <map>
using namespace std;

/*token类型*/
enum TokenType
{
    TK_ID,      //标识符
    TK_NUM,     //数字
    TK_STRING,  //字符串
    TK_CHAR,    //字符
    TK_ASSIGN,  //=
    TK_EQ,      //==
    TK_NEQ,     //!=
    TK_GT,      //>
    TK_GEQ,     //>=
    TK_LT,      //<
    TK_LEQ,     //<=
    TK_ADD,     //+
    TK_SUB,     //-
    TK_STAR,    //*
    TK_DIV,     // /
    TK_MOD,     //%
    TK_LLITTLE, //(
    TK_RLITTLE, //)
    TK_LMIDDLE, //[
    TK_RMIDDLE, //]
    TK_LLARGE,  //{
    TK_RLARGE,  //}
    TK_POINT,   //.
    TK_COMMA,   //,
    TK_SEMI,    //;
    TK_WELL,    //#
    TK_NOT,     //!
    TK_COLON,   //:
    TK_OR,      //|
    TK_AND,     //&
    TK_QUES,    //?
    TK_LDI,     //~
    TK_AT,      //@
    TK_FILEEND, //文件结束
};

map<TokenType, string> tk_str_map = {
    // TokenType与字符串的转换
    {TK_ID, "id"},
    {TK_NUM, "number"},
    {TK_STRING, "string"},
    {TK_CHAR, "char"},
    {TK_ASSIGN, "="},
    {TK_EQ, "=="},
    {TK_NEQ, "!="},
    {TK_GT, ">"},
    {TK_GEQ, ">="},
    {TK_LT, "<"},
    {TK_LEQ, "<="},
    {TK_ADD, "+"},
    {TK_SUB, "-"},
    {TK_STAR, "*"},
    {TK_DIV, "/"},
    {TK_MOD, "%"},
    {TK_LLITTLE, "("},
    {TK_RLITTLE, ")"},
    {TK_LMIDDLE, "["},
    {TK_RMIDDLE, "]"},
    {TK_LLARGE, "{"},
    {TK_RLARGE, "}"},
    {TK_POINT, "."},
    {TK_COMMA, ","},
    {TK_SEMI, ";"},
    {TK_WELL, "#"},
    {TK_NOT, "!"},
    {TK_COLON, ":"},
    {TK_OR, "|"},
    {TK_AND, "&"},
    {TK_QUES, "?"},
    {TK_LDI, "~"},
    {TK_AT, "@"},
    {TK_FILEEND, "eof"},
};

/*
存储token的信息
*/
class TokenInfo
{
public:
    TokenInfo(TokenType, int, int, string);
    string toString(); //转化成字符串

    int line, column;
    TokenType type;
    string str;
};

TokenInfo::TokenInfo(TokenType type, int line, int column, string str) : line(line), column(column), type(type), str(str)
{
}

string TokenInfo::toString()
{
    return "TokenInfo(" + to_string(type) + "(" + +"\"" + tk_str_map[type] + "\"" + ")" + "," + to_string(line) + "," + to_string(column) + "," + +"\"" + str + "\"" + ")";
}