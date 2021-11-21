#include "lexer.h"

void Lexer_Init()
{
	keywordmap = DictObject_New();
	DictObject_SetItem((struct Object*)keywordmap, StringObject_NewWithString("if"), IntObject_NewWithValue(TK_IF));
	DictObject_SetItem((struct Object*)keywordmap, StringObject_NewWithString("and"), IntObject_NewWithValue(TK_AND));
	DictObject_SetItem((struct Object*)keywordmap, StringObject_NewWithString("or"), IntObject_NewWithValue(TK_OR));
	DictObject_SetItem((struct Object*)keywordmap, StringObject_NewWithString("else"), IntObject_NewWithValue(TK_ELSE));
	DictObject_SetItem((struct Object*)keywordmap, StringObject_NewWithString("def"), IntObject_NewWithValue(TK_DEF));
	DictObject_SetItem((struct Object*)keywordmap, StringObject_NewWithString("return"), IntObject_NewWithValue(TK_RETURN));
	DictObject_SetItem((struct Object*)keywordmap, StringObject_NewWithString("begin"), IntObject_NewWithValue(TK_BEGIN));
	DictObject_SetItem((struct Object*)keywordmap, StringObject_NewWithString("end"), IntObject_NewWithValue(TK_END));
	DictObject_SetItem((struct Object*)keywordmap, StringObject_NewWithString("while"), IntObject_NewWithValue(TK_WHILE));
	DictObject_SetItem((struct Object*)keywordmap, StringObject_NewWithString("break"), IntObject_NewWithValue(TK_BREAK));
	DictObject_SetItem((struct Object*)keywordmap, StringObject_NewWithString("continue"), IntObject_NewWithValue(TK_CONTINUE));
}

struct Lexer* Lexer_New(struct StringObject* code)
{
	struct Lexer* lexer = (struct Lexer*)malloc(sizeof(struct Lexer));
	if (lexer == NULL)
	{
		printf("Lexer分配内存失败");
		exit(-1);
	}
	lexer->code = code;
	lexer->lineno = 1;
	lexer->linepos = 1;
	lexer->pos = 0;
	lexer->state = ST_START;
	lexer->eof = 0;
	lexer->indent=StringObject_New();
	Lexer_getch(lexer);
	Lexer_gettoken(lexer);
	return lexer;
}

void Lexer_getch(struct Lexer* lexer)
{
	if (lexer->pos < lexer->code->size)
	{
		lexer->ch = lexer->code->string[lexer->pos++];
		if (lexer->ch == '\n')
		{
			lexer->lineno++;
			lexer->linepos = 0;
		}
	}
	else
	{
		lexer->ch = EOF;
		lexer->eof = 1;
	}
}

void Lexer_ungetch(struct Lexer* lexer)
{
	if (!lexer->eof)
	{
		lexer->pos--;
	}
}

void Lexer_gettoken(struct Lexer* lexer)
{
	lexer->tokenstr = (struct StringObject*)StringObject_New();
	lexer->state = ST_START;
	int flag = 0;	//是否将ch添加到tokenstr中
	int end = 0;	//是否结束
	while (1)
	{
		flag = 0;
		switch (lexer->state)
		{
		case ST_START:
			if (isName(lexer->ch))
			{
				lexer->state = ST_NAME;
				flag = 1;
			}
			else if (lexer->ch == '{')
			{
				lexer->token = TK_LLARGE;
				end = 1;
				flag = 1;
			}
			else if (lexer->ch == '}')
			{
				lexer->token = TK_RLARGE;
				end = 1;
				flag = 1;
			}
			else if (lexer->ch == EOF)
			{
				lexer->token = TK_EOF;
				end = 1;
			}
			else if (lexer->ch == '=')
			{
				lexer->state = ST_EQ;
				flag = 1;
			}
			else if (lexer->ch == '>')
			{
				lexer->state = ST_GT;
				flag = 1;
			}
			else if (lexer->ch == '<')
			{
				lexer->state = ST_LT;
				flag = 1;
			}
			else if (lexer->ch == '!')
			{
				lexer->state = ST_NOT;
				flag = 1;
			}
			else if (lexer->ch == '+')
			{
				lexer->token = TK_ADD;
				flag = 1;
				end = 1;
			}
			else if (lexer->ch == '-')
			{
				lexer->token = TK_SUB;
				flag = 1;
				end = 1;
			}
			else if (lexer->ch == '*')
			{
				lexer->token = TK_MUL;
				flag = 1;
				end = 1;
			}
			else if (lexer->ch == '/')
			{
				lexer->token = TK_DIV;
				flag = 1;
				end = 1;
			}
			else if (lexer->ch == '%')
			{
				lexer->token = TK_MOD;
				flag = 1;
				end = 1;
			}
			else if (isNum(lexer->ch))
			{
				lexer->state = ST_NUM;
				flag = 1;
			}
			else if (lexer->ch == '(')
			{
				lexer->token = TK_LLITTLE;
				flag = 1;
				end = 1;
			}
			else if (lexer->ch == ')')
			{
				lexer->token = TK_RLITTLE;
				flag = 1;
				end = 1;
			}
			else if (lexer->ch == '[')
			{
				lexer->token = TK_LMIDDLE;
				flag = 1;
				end = 1;
			}
			else if (lexer->ch == ']')
			{
				lexer->token = TK_RMIDDLE;
				flag = 1;
				end = 1;
			}
			else if (lexer->ch == ',')
			{
				lexer->token = TK_COMMA;
				flag = 1;
				end = 1;
			}
			else if (lexer->ch == '\n')
			{
				lexer->indent = StringObject_New();
				lexer->state = ST_INDENT;
			}
			else if (lexer->ch == ' ' || lexer->ch == '\t')
			{

			}
			else if (lexer->ch == ':')
			{
				lexer->token = TK_COLON;
				flag = 1;
				end = 1;
			}
			else if (lexer->ch == '.')
			{
				lexer->token = TK_POINT;
				flag = 1;
				end = 1;
			}
			else if (lexer->ch == '\'')
			{
				lexer->state = ST_STRING1;
			}
			else if (lexer->ch == '"')
			{
				lexer->state = ST_STRING2;
			}
			break;
		case ST_NAME:
			if (isName(lexer->ch))
			{
				flag = 1;
			}
			else
			{
				Lexer_ungetch(lexer);
				lexer->token = TK_NAME;
				int index = DictObject_FindItem((struct Object*)keywordmap, (struct Object*)lexer->tokenstr);
				if (index != -1)
				{
					lexer->token = IntObject_toInt(DictObject_GetItem((struct Object*)keywordmap, (struct Object*)lexer->tokenstr));
				}
				end = 1;
			}
			break;
		case ST_NUM:
			if (isNum(lexer->ch))
			{
				flag = 1;
			}
			else if (lexer->ch == '.')
			{
				flag = 1;
				lexer->state = ST_DOUBLE;
			}
			else
			{
				Lexer_ungetch(lexer);
				lexer->token = TK_NUM;
				end = 1;
			}
			break;
		case ST_DOUBLE:
			if (isNum(lexer->ch))
			{
				flag = 1;
			}
			else
			{
				Lexer_ungetch(lexer);
				lexer->token = TK_DOUBLE;
				end = 1;
			}
			break;
		case ST_EQ:
			if (lexer->ch == '=')
			{
				lexer->token = TK_EQ;
				end = 1;
				flag = 1;
			}
			else
			{
				lexer->token = TK_ASSIGN;
				Lexer_ungetch(lexer);
				end = 1;
			}
			break;
		case ST_NOT:
			if (lexer->ch == '=')
			{
				lexer->token = TK_NEQ;
				end = 1;
				flag = 1;
			}
			else
			{
				printf("暂不支持!操作符");
				exit(-1);
			}
			break;
		case ST_GT:
			if (lexer->ch == '=')
			{
				lexer->token = TK_GEQ;
				end = 1;
				flag = 1;
			}
			else
			{
				lexer->token = TK_GT;
				Lexer_ungetch(lexer);
				end = 1;
			}
			break;
		case ST_LT:
			if (lexer->ch == '=')
			{
				lexer->token = TK_LEQ;
				end = 1;
				flag = 1;
			}
			else
			{
				lexer->token = TK_LT;
				Lexer_ungetch(lexer);
				end = 1;
			}
			break;
		case ST_INDENT:
			if (lexer->ch == ' ' || lexer->ch == '\t')
			{
				flag = 1;
				lexer->indent = StringObject_Add(lexer->indent, StringObject_NewWithChar(lexer->ch));
			}
			else if(lexer->ch=='\n')
			{
				lexer->tokenstr = StringObject_New();	//重新计算缩进
			}
			else
			{
				lexer->token = TK_INDENT;
				Lexer_ungetch(lexer);
				end = 1;
			}
			break;
		case ST_STRING1:
			if (lexer->ch == '\'')
			{
				end = 1;
				lexer->token = TK_STRING;
			}
			else if (lexer->ch == '\\')
			{
				Lexer_getch(lexer);
				Lexer_escape(lexer);
				flag = 1;
			}
			else
			{
				flag = 1;
			}
			break;
		case ST_STRING2:
			if (lexer->ch == '"')
			{
				end = 1;
				lexer->token = TK_STRING;
			}
			else if (lexer->ch == '\\')
			{
				Lexer_getch(lexer);
				Lexer_escape(lexer);
				flag = 1;
			}
			else
			{
				flag = 1;
			}
			break;
		}
		if (flag)
		{
			lexer->tokenstr = (struct StringObject*)StringObject_Add((struct Object*)lexer->tokenstr, StringObject_NewWithChar(lexer->ch));
		}
		Lexer_getch(lexer);
		if (end)
		{
			break;
		}
	}
	if (lexer->token == TK_INDENT)
	{
		Lexer_gettoken(lexer);
	}
}

void Lexer_escape(struct Lexer* lexer)
{
	switch (lexer->ch)
	{
	case 'n':
		lexer->ch = '\n';
		break;
	case 'r':
		lexer->ch = '\r';
		break;
	case '\\':
		lexer->ch = '\\';
		break;
	case 'b':
		lexer->ch = '\b';
		break;
	}
}