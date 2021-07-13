#include "Include/lexer.h"
#include "Include/globals.h"
#include "Include/dynstring.h"
#include "Include/error.h"

struct Lexer* lexer_init(char* code,char *filename)
{
	struct Lexer* lexer = (struct Lexer*)malloc(sizeof(struct Lexer));
	if (NULL == lexer)
	{
		printf("lexer初始化失败");
		exit(-1);
	}
	lexer->code = code;
	lexer->filename = filename;
	lexer->codelen = strlen(code);
	lexer->lineno = 1;
	lexer->linepos = 0;
	lexer->pos = 0;
	lexer->indent = "\0";
	lexer->eof = 0;
	lexer->tokenstr = "\0";
	return lexer;
}

void getch(struct Lexer* lexer)
{
	if (lexer->pos < lexer->codelen)
	{
		if (lexer->code[lexer->pos] == '\n')
		{
			lexer->lineno++;
			lexer->linepos = 0;
		}
		lexer->linepos++;
		lexer->ch = lexer->code[lexer->pos++];
	}
	else
	{
		lexer->eof = 1;
		lexer->ch = EOF;
	}
}
void ungetch(struct Lexer* lexer)
{
	if (!lexer->eof)
	{
		lexer->pos--;
		lexer->linepos++;
	}
}

void gettoken(struct Lexer* lexer)
{
	while (lexer->ch!=EOF&&(lexer->ch == ' ' || lexer->ch == '\n' || lexer->ch == '\t'))
	{
		if (lexer->ch == '\n'&&blockstyle==INDENT)
		{
			getindent(lexer);
		}
		else
		{
			getch(lexer);
		}
	}
	if (isValue(lexer->ch))
	{
		getname(lexer);
	}
	else if (isNum(lexer->ch))
	{
		getnum(lexer);
	}
	else if (lexer->ch == '+')
	{
		lexer->token = TK_ADD;
		getch(lexer);
	}
	else if (lexer->ch == '-')
	{
		lexer->token = TK_SUB;
		getch(lexer);
	}
	else if (lexer->ch == '*')
	{
		lexer->token = TK_MUL;
		getch(lexer);
	}
	else if (lexer->ch == '/')
	{
		lexer->token = TK_DIV;
		getch(lexer);
	}
	else if (lexer->ch == '%')
	{
		lexer->token = TK_MOD;
		getch(lexer);
	}
	else if (lexer->ch == '>')
	{
		getch(lexer);
		if (lexer->ch == '=')
		{
			lexer->token = TK_GEQ;
			getch(lexer);
		}
		else if (lexer->ch == '>')
		{
			lexer->token = TK_RSHIFT;
			getch(lexer);
		}
		else
		{
			lexer->token = TK_GT;
		}
	}
	else if (lexer->ch == '<')
	{
		getch(lexer);
		if (lexer->ch == '=')
		{
			lexer->token = TK_LEQ;
			getch(lexer);
		}
		else if (lexer->ch == '<')
		{
			lexer->token = TK_LSHIFT;
			getch(lexer);
		}
		else
		{
			lexer->token = TK_LT;
		}
	}
	else if (lexer->ch == '=')
	{
		getch(lexer);
		if (lexer->ch == '=')
		{
			lexer->token = TK_EQ;
			getch(lexer);
		}
		else
		{
			lexer->token = TK_ASSIGN;
		}
	}
	else if (lexer->ch == '!')
	{
		getch(lexer);
		if (lexer->ch == '=')
		{
			lexer->token = TK_NEQ;
			getch(lexer);
		}
		else
		{
			error("暂不支持'!'操作符", lexer->filename, lexer->lineno, lexer->linepos);
		}
	}
	else if (lexer->ch == ';')
	{
		lexer->token = TK_SEMI;
		getch(lexer);
	}
	else if (lexer->ch == ',')
	{
		lexer->token = TK_COMMA;
		getch(lexer);
	}
	else if (lexer->ch == ':')
	{
		lexer->token = TK_COLON;
		getch(lexer);
	}
	else if (lexer->ch == '(')
	{
		lexer->token = TK_LLITTLE;
		getch(lexer);
	}
	else if (lexer->ch == ')')
	{
		lexer->token = TK_RLITTLE;
		getch(lexer);
	}
	else if (lexer->ch == '[')
	{
		lexer->token = TK_LMIDDLE;
		getch(lexer);
	}
	else if (lexer->ch == ']')
	{
		lexer->token = TK_RMIDDLE;
		getch(lexer);
	}
	else if (lexer->ch == '{')
	{
		lexer->token = TK_LLARGE;
		getch(lexer);
	}
	else if (lexer->ch == '}')
	{
		lexer->token = TK_RLARGE;
		getch(lexer);
	}
	else if (lexer->ch == '"')
	{
		getstring(lexer);
	}
	else if (lexer->ch == '\'')
	{
		getletter(lexer);
	}
	else if (lexer->ch == '.')
	{
		lexer->token = TK_POINT;
		getch(lexer);
	}
	else if (lexer->ch == EOF)
	{
		lexer->token = TK_EOF;
	}
	else
	{
		error("未知字符'%c'", lexer->filename, lexer->lineno, lexer->linepos, lexer->ch);
	}
}

void getname(struct Lexer* lexer)
{
	struct DynString* name = dynstring_init();
	while (isValue(lexer->ch) || isNum(lexer->ch))
	{
		dynstring_add_chr(name, lexer->ch);
		getch(lexer);
	}
	lexer->tokenstr = (char*)malloc(sizeof(char) * strlen(name->str));
	if (0 == lexer->tokenstr)
	{
		printf("lexer->tokenstr为0");
		exit(-1);
	}
	strcpy(lexer->tokenstr, name->str);
	for (int i = 0; i < MAXTOKENMAPLEN; i++)
	{
		if (!strcmp(tokenmap[i].str, name->str))
		{
			lexer->token = tokenmap[i].tk;
			return;
		}
	}
	lexer->token = TK_NAME;
}

void getnum(struct Lexer* lexer)
{
	struct DynString* num = dynstring_init();
	while (isNum(lexer->ch))
	{
		dynstring_add_chr(num, lexer->ch);
		getch(lexer);
	}
	if (lexer->ch == '.')	//小数
	{
		dynstring_add_chr(num, lexer->ch);
		getch(lexer);
		while (isNum(lexer->ch))
		{
			dynstring_add_chr(num, lexer->ch);
			getch(lexer);
		}
	}
	lexer->tokenstr = (char*)malloc(sizeof(char) * strlen(num->str));
	if (0 == lexer->tokenstr)
	{
		printf("lexer->tokenstr为0");
		exit(-1);
	}
	strcpy(lexer->tokenstr, num->str);
	lexer->token = TK_NUM;
}

void getstring(struct Lexer* lexer)
{
	getch(lexer);
	struct DynString* string = dynstring_init();
	while (lexer->ch != EOF && lexer->ch != '"')
	{
		if (lexer->ch == '\\')
		{
			getch(lexer);
			getescape(lexer);
		}
		dynstring_add_chr(string, lexer->ch);
		getch(lexer);
	}
	getch(lexer);
	lexer->tokenstr = (char*)malloc(sizeof(char) * strlen(string->str));
	if (0 == lexer->tokenstr)
	{
		printf("lexer->tokenstr为0");
		exit(-1);
	}
	strcpy(lexer->tokenstr, string->str);
	lexer->token = TK_STRING;
}

void getletter(struct Lexer* lexer)
{
	getch(lexer);
	struct DynString* string = dynstring_init();
	if (lexer->ch == '\\')
	{
		getch(lexer);
		getescape(lexer);
	}
	dynstring_add_chr(string, lexer->ch);
	getch(lexer);
	if (lexer->ch != '\'')
	{
		error("只能有一个字符", lexer->filename, lexer->lineno, lexer->linepos);
	}
	getch(lexer);
	lexer->tokenstr = (char*)malloc(sizeof(char) * strlen(string->str));
	if (0 == lexer->tokenstr)
	{
		printf("lexer->tokenstr为0");
		exit(-1);
	}
	strcpy(lexer->tokenstr, string->str);
	lexer->token = TK_LETTER;
}

void getescape(struct Lexer* lexer)
{
	switch (lexer->ch)
	{
	case 'n':
		lexer->ch = '\n';
		break;
	case 't':
		lexer->ch = '\t';
		break;
	case '"':
		lexer->ch = '"';
		break;
	case '\'':
		lexer->ch = '\'';
		break;
	case '\\':
		lexer->ch = '\\';
		break;
	default:
		error("未知转义字符'\\%c'", lexer->filename, lexer->lineno, lexer->linepos, lexer->ch);
	}
}

void getindent(struct Lexer* lexer)
{
	getch(lexer);
	struct DynString* indent = dynstring_init();
	while (lexer->ch != EOF && (lexer->ch == ' ' || lexer->ch == '\t'))
	{
		dynstring_add_chr(indent, lexer->ch);
		getch(lexer);
	}
	/*如果是空行则重新获取缩进*/
	if (lexer->ch == '\n')
	{
		getindent(lexer);
	}
	lexer->indent = (char*)malloc(sizeof(char) * strlen(indent->str));
	if (0 == lexer->indent)
	{
		printf("lexer->indent为0");
		exit(-1);
	}
	strcpy(lexer->indent, indent->str);
}