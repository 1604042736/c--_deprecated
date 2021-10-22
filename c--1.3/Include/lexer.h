#pragma once

#include <string>
#include <iostream>

#include "globals.h"
using namespace std;

#define MAXTOKENMAPLEN 11

#define isName(c)	((c>='a'&&c<='z')||(c>='A'&&c<='Z'))||c=='_'
#define isNum(c)	(c>='0'&&c<='9')

enum Token
{
	TK_NAME,	//±êÊ¶·û
	TK_NUM,	//Êý×Ö
	TK_STRING,//×Ö·û´®
	TK_LLARGE,	//{
	TK_RLARGE,	//}
	TK_GT,	// >
	TK_GEQ,	// >=
	TK_LT,	// <
	TK_LEQ,	// <=
	TK_EQ,	// ==
	TK_NEQ,	// !=
	TK_ASSIGN,	//=
	TK_LSHIFT,	//<<
	TK_RSHIFT,	//>>
	TK_ADD,	// +
	TK_SUB,	// -
	TK_MUL,	// *
	TK_DIV,	// /
	TK_MOD,	// %
	TK_SEMI,	// ;
	TK_COMMA,	// ,
	TK_POINT,	// .
	TK_COLON,	// :
	TK_LLITTLE,	//( 
	TK_RLITTLE,	//)
	TK_LMIDDLE, //[
	TK_RMIDDLE,	//]
	TK_IF,TK_ELSE,TK_OR,TK_AND,TK_WHILE,TK_DEF,TK_BREAK, TK_CONTINUE,TK_RETURN,TK_BEGIN,TK_END,
	TK_EOF
};

struct Lexer
{
	string content;
	string filename;
	int lineno, linepos,pos;
	Token token;
	string tokenstr;
	bool eof;
	char ch;
	string indent;
	BlockStyle blockstyle;
};

static struct TokenMap
{
	string str;
	enum Token tk;
}tokenmap[MAXTOKENMAPLEN] = { {"if",TK_IF},{"else",TK_ELSE} ,{"or",TK_OR},{"and",TK_AND},{"while",TK_WHILE},{"def",TK_DEF},
	{"break",TK_BREAK},{"contiune",TK_CONTINUE},{"return",TK_RETURN} ,{"begin",TK_BEGIN},{"end",TK_END} };

Lexer* lexer_init(string,string);
void lexer_getch(Lexer*);
void lexer_ungetch(Lexer*);
void lexer_gettoken(Lexer*);
void lexer_getname(Lexer*);
void lexer_getnum(Lexer*);
void lexer_getindent(Lexer*);
void lexer_getstring(Lexer*,char);