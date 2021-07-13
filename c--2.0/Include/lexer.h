#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define isValue(c)	((c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_')
#define isNum(c)	(c>='0'&&c<='9')
#define MAXTOKENMAPLEN	(TK_KEYWORDEND-TK_KEYWORDBEGIN-1)

enum Token
{
	TK_NAME,	//变量名
	TK_NUM,	//数字
	TK_ADD,	// +
	TK_SUB,	// -
	TK_MUL,	// *
	TK_DIV,	// /
	TK_MOD,	// %
	TK_GT,	// >
	TK_GEQ,	// >=
	TK_LT,	// <
	TK_LEQ,	// <=
	TK_EQ,	// ==
	TK_NEQ,	// !=
	TK_ASSIGN,	// =
	TK_SEMI,	// ;
	TK_COMMA,	// ,
	TK_POINT,	// .
	TK_COLON,	// :
	TK_LSHIFT,	//<<
	TK_RSHIFT,	//>>
	TK_STRING,TK_LETTER,
	TK_LLITTLE,TK_RLITTLE,	// ()
	TK_LMIDDLE,TK_RMIDDLE,	// []
	TK_LLARGE,TK_RLARGE,	// {}
	TK_KEYWORDBEGIN,	//无实际意义,用来标识关键词开始位置
	TK_IF,TK_WHILE,TK_BREAK,TK_CONTINUE,TK_ELSE,TK_INT,TK_CHAR,TK_VOID,TK_AND,TK_OR,TK_BEGIN,TK_END,TK_RETURN,TK_STRUCT,
	TK_KEYWORDEND,	//无实际意义,用来标识关键词结束位置
	TK_EOF
};

/*关键词token与字符串的关系*/
static struct TokenMap
{
	char* str;
	enum Token tk;
}tokenmap[MAXTOKENMAPLEN] = { {"if",TK_IF},{"else",TK_ELSE},{"while",TK_WHILE},{"break",TK_BREAK},{"continue",TK_CONTINUE},
							  {"int",TK_INT},{"char",TK_CHAR},{"void",TK_VOID},{"and",TK_AND},{"or",TK_OR},{"begin",TK_BEGIN},
							  {"end",TK_END},{"return",TK_RETURN},{"struct",TK_STRUCT} };

struct Lexer
{
	char* code;	//代码
	char* filename;	//文件名
	int codelen;	//代码长度
	int linepos;	//当前列
	int lineno;	//当前行
	int pos;	//当前索引
	int eof;	//是否结束
	char* indent;	//缩进
	char* tokenstr;	//token对应的字符串
	char ch;
	enum Token token;
};

struct Lexer* lexer_init(char *code,char* filename);
void getch(struct Lexer* lexer);
void ungetch(struct Lexer* lexer);
void gettoken(struct Lexer* lexer);
void getname(struct Lexer* lexer);
void getnum(struct Lexer* lexer);
void getindent(struct Lexer* lexer);
void getstring(struct Lexer* lexer);
void getletter(struct Lexer* lexer);
void getescape(struct Lexer* lexer);