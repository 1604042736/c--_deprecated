#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define CHECK_MALLOC(a) if(a==NULL){printf("%s:%d:分配内存失败",__FILE__,__LINE__);exit(1);}

enum TokenType
{
	TK_EOF,
	TK_NAME, TK_INT, TK_DEC, TK_STRING, TK_CHAR,
	TK_ASSIGN,
	TK_PLUS, TK_MINUS, TK_STAR, TK_DIV, TK_MOD,
	TK_BITAND, TK_BITOR, TK_BITXOR, TK_LSHIFT, TK_RSHIFT,
	TK_LLITTLE, TK_RLITTLE,
	TK_LMIDDLE, TK_RMIDDLE,
	TK_LLARGE, TK_RLARGE,
	TK_EQ, TK_NEQ, TK_GT, TK_GE, TK_LT, TK_LE,
	TK_SEMI, TK_COLON, TK_COMMA, TK_POINT,
	TK_ELLIPSIS,

	KW_INT, KW_CHAR, KW_IF, KW_WHILE, KW_FOR, KW_VAR, KW_FUNC, KW_RETURN, KW_BREAK, KW_CONTINUE,
	KW_AND, KW_OR, KW_ELSE, KW_VOID, KW_EXTERN, KW_STRUCT
};

enum State
{
	S_START,
	S_NAME, S_INT, S_DEC, S_STRING, S_CHAR,
	S_ASSIGN, S_GT, S_LT, S_EXCLA,
	S_ESCAPE,
	S_ELLIPSIS1,S_ELLIPSIS2,
	S_PRE,S_PRENAME
};

#define TOKENSTRMAP_LEN 43
static struct
{
	enum Tokentype token;
	char* str;
}tokenstrmap[] = {
	{TK_ASSIGN,"="},
	{TK_PLUS,"+"},
	{TK_MINUS,"-"},
	{TK_STAR,"*"},
	{TK_DIV,"/"},
	{TK_MOD,"%"},
	{TK_BITAND,"&"},
	{TK_BITOR,"|"},
	{TK_BITXOR,"^"},
	{TK_LSHIFT,"<<"},
	{TK_RSHIFT,">>"},
	{TK_LLITTLE,"("},
	{TK_RLITTLE,")"},
	{TK_LMIDDLE,"["},
	{TK_RMIDDLE,"]"},
	{TK_LLARGE,"{"},
	{TK_RLARGE,"}"},
	{TK_EQ,"=="},
	{TK_NEQ,"!="},
	{TK_GT,">"},
	{TK_GE,">="},
	{TK_LT,"<"},
	{TK_LE,"<="},
	{TK_SEMI,";"},
	{TK_COLON,":"},
	{TK_COMMA,","},
	{TK_POINT,"."},
	{TK_ELLIPSIS,"..."},
	{KW_INT,"int"},
	{KW_CHAR,"char"},
	{KW_IF,"if"},
	{KW_WHILE,"while"},
	{KW_FOR,"for"},
	{KW_VAR,"var"},
	{KW_FUNC,"func"},
	{KW_RETURN,"return"},
	{KW_BREAK,"break"},
	{KW_CONTINUE,"continue"},
	{KW_AND,"and"},
	{KW_OR,"or"},
	{KW_ELSE,"else"},
	{KW_VOID,"void"},
	{KW_EXTERN,"extern"},
	{KW_STRUCT,"struct"}
};

struct Lexer
{
	enum TokenType token;
	char* tokenstr;
	int line, column;
	int start_column;
	char* linecode;
	char* filename;
	FILE* file;
};

struct Lexer* lexer_new(char*);
char lexer_getch(struct Lexer*);
void lexer_ungetch(struct Lexer*);
void lexer_gettoken(struct Lexer*);
void lexer_error(struct Lexer*, char*, ...);
void lexer_error2(struct Lexer*, char*, va_list);
void lexer_warning(struct Lexer*, char*, ...);
char get_escape(char);
char* get_tokenstr(enum TokenType);