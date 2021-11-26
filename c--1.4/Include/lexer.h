#pragma once

#include "dictobject.h"
#include "stringobject.h"
#include "intobject.h"
#include "preprocessor.h"

#define isName(ch)	((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||ch=='_')
#define isNum(ch)	(ch>='0'&&ch<='9')

#define TK_NAME		0
#define TK_NUM		1
#define TK_IF		2
#define TK_LLARGE	3
#define TK_RLARGE	4
#define TK_EOF		5
#define TK_AND		6
#define TK_OR		7
#define TK_EQ		8
#define TK_NEQ		9
#define TK_GT		10
#define TK_GEQ		11
#define TK_LT		12
#define TK_LEQ		13
#define TK_ASSIGN	15
#define TK_ADD		16
#define TK_SUB		17
#define TK_MUL		18
#define TK_DIV		19
#define TK_MOD		20
#define TK_ELSE		21
#define TK_LLITTLE	22
#define TK_RLITTLE	23
#define TK_LMIDDLE	24
#define TK_RMIDDLE	25
#define TK_COMMA	26
#define TK_DEF		27
#define TK_RETURN	28
#define TK_INDENT	29
#define TK_COLON	30
#define TK_BEGIN	31
#define TK_END		32
#define TK_WHILE	33
#define TK_POINT	34
#define TK_STRING	35
#define TK_DOUBLE	36
#define TK_BREAK	37
#define TK_CONTINUE	38
#define TK_COMMENT	39
#define TK_IMPORT	40
#define TK_CLASS	41
#define TK_TRY		42
#define TK_EXCEPT	43

enum State
{
	ST_START,ST_NAME,ST_EQ,ST_GT,ST_LT,ST_NOT,ST_NUM,ST_INDENT,ST_STRING1,ST_STRING2,ST_DOUBLE,ST_NAMESPACEATTR
};

struct Lexer
{
	enum State state;
	int token;
	int lineno, linepos;	//行列
	int pos;	//当前字符位置
	char ch;
	int eof;
	struct StringObject* filename;
	struct StringObject* tokenstr;
	struct StringObject* code;
	struct StringObject* indent;
};

static struct DictObject* keywordmap;
void Lexer_Init();
struct Lexer* Lexer_New(struct Preprocessor*);
void Lexer_gettoken(struct Lexer*);
void Lexer_getch(struct Lexer*);
void Lexer_ungetch(struct Lexer*);
void Lexer_escape(struct Lexer*);