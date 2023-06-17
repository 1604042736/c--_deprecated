#include "lexer.h"

struct Lexer* lexer_new(char* filename)
{
	struct Lexer* lexer = (struct Lexer*)malloc(sizeof(struct Lexer));
	if (lexer == NULL)
	{
		printf("创建Lexer失败");
		exit(1);
	}
	lexer->file = fopen(filename, "r");
	lexer->filename = filename;
	lexer->line = 0;
	lexer->column = 0;
	lexer->linecode = "\0";
	lexer->tokenstr = "\0";
	lexer->start_column = 0;
	return lexer;
}

char lexer_getch(struct Lexer* lexer)
{
	if (lexer->column >= strlen(lexer->linecode))
	{
		char buff[1024];
		lexer->linecode = "\0";
		do	//保证读取完整一行代码
		{
			if (fgets(buff, 1024, lexer->file) == NULL)
			{
				break;
			}
			char* old = lexer->linecode;
			lexer->linecode = (char*)malloc(sizeof(char) * (strlen(old) + strlen(buff)));
			CHECK_MALLOC(lexer->linecode);
			strcpy(lexer->linecode, old);
			strcat(lexer->linecode, buff);
		} while (buff[strlen(buff) - 1] != '\n');
		if (strlen(lexer->linecode) == 0)
		{
			return EOF;
		}
		lexer->line += 1;
		lexer->column = 0;
	}
	return lexer->linecode[lexer->column++];
}

void lexer_ungetch(struct Lexer* lexer)
{
	if (lexer->column - 1 >= 0)
	{
		lexer->column -= 1;
	}
}

void lexer_gettoken(struct Lexer* lexer)
{
	enum State state = S_START;
	int max_tokenstr_len = 1024;
	lexer->tokenstr = (char*)malloc(sizeof(char) * max_tokenstr_len);
	CHECK_MALLOC(lexer->tokenstr);
	lexer->tokenstr[0] = '\0';
	int p_tokenstr = 0;
	while (1)
	{
		char ch = lexer_getch(lexer);
		switch (state)
		{
		case S_START:
		{
			lexer->start_column = lexer->column;
			if (isalpha(ch))
			{
				state = S_NAME;
			}
			else if (isdigit(ch))
			{
				state = S_INT;
			}
			else if (ch == '\'')
			{
				state = S_CHAR;
				goto notstore;
			}
			else if (ch == '"')
			{
				state = S_STRING;
				goto notstore;
			}
			else if (ch == '=')
			{
				state = S_ASSIGN;
			}
			else if (ch == '!')
			{
				state = S_EXCLA;
			}
			else if (ch == '>')
			{
				state = S_GT;
			}
			else if (ch == '<')
			{
				state = S_LT;
			}
			else if (ch == '+')
			{
				lexer->token = TK_PLUS;
				goto storeandquit;
			}
			else if (ch == '-')
			{
				lexer->token = TK_MINUS;
				goto storeandquit;
			}
			else if (ch == '*')
			{
				lexer->token = TK_STAR;
				goto storeandquit;
			}
			else if (ch == '/')
			{
				lexer->token = TK_DIV;
				goto storeandquit;
			}
			else if (ch == '%')
			{
				lexer->token = TK_MOD;
				goto storeandquit;
			}
			else if (ch == '&')
			{
				lexer->token = TK_BITAND;
				goto storeandquit;
			}
			else if (ch == '|')
			{
				lexer->token = TK_BITOR;
				goto storeandquit;
			}
			else if (ch == '(')
			{
				lexer->token = TK_LLITTLE;
				goto storeandquit;
			}
			else if (ch == ')')
			{
				lexer->token = TK_RLITTLE;
				goto storeandquit;
			}
			else if (ch == '[')
			{
				lexer->token = TK_LMIDDLE;
				goto storeandquit;
			}
			else if (ch == ']')
			{
				lexer->token = TK_RMIDDLE;
				goto storeandquit;
			}
			else if (ch == '{')
			{
				lexer->token = TK_LLARGE;
				goto storeandquit;
			}
			else if (ch == '}')
			{
				lexer->token = TK_RLARGE;
				goto storeandquit;
			}
			else if (ch == ';')
			{
				lexer->token = TK_SEMI;
				goto storeandquit;
			}
			else if (ch == ':')
			{
				lexer->token = TK_COLON;
				goto storeandquit;
			}
			else if (ch == ',')
			{
				lexer->token = TK_COMMA;
				goto storeandquit;
			}
			else if (ch == '^')
			{
				lexer->token = TK_BITXOR;
				goto storeandquit;
			}
			else if (ch == '.')
			{
				state = S_ELLIPSIS1;
			}
			else if (ch == EOF)
			{
				lexer->token = TK_EOF;
				goto quit;
			}
			else if (isspace(ch))
			{
				continue;
			}
			else if (ch == '#')
			{
				state = S_PRE;
				goto notstore;
			}
			else
			{
				lexer_error(lexer, "Unresolved characters");
			}
			break;
		}
		case S_NAME:
		{
			lexer->token = TK_NAME;
			if (!(isdigit(ch) || isalpha(ch) || ch == '_'))
			{
				for (int i = 0; i < TOKENSTRMAP_LEN; i++)
				{
					if (!strcmp(tokenstrmap[i].str, lexer->tokenstr))
					{
						lexer->token = tokenstrmap[i].token;
						break;
					}
				}
				lexer_ungetch(lexer);
				goto quit;
			}
			break;
		}
		case S_INT:
		{
			lexer->token = TK_INT;
			if (ch == '.')
			{
				state = S_DEC;
			}
			else if (!isdigit(ch))
			{
				lexer_ungetch(lexer);
				goto quit;
			}
			break;
		}
		case S_DEC:
		{
			lexer->token = TK_DEC;
			if (!isdigit(ch))
			{
				lexer_ungetch(lexer);
				goto quit;
			}
			break;
		}
		case S_STRING:
		{
			lexer->token = TK_STRING;
			if (ch == '"')
			{
				goto quit;
			}
			else if (ch == '\\')
			{
				char next = lexer_getch(lexer);
				ch = get_escape(next);
				if (ch == next)
				{
					lexer_warning(lexer, "未知的转义字符");
					lexer_ungetch(lexer);
					ch = '\\';
				}
			}
			break;
		}
		case S_CHAR:
		{
			lexer->token = TK_CHAR;
			if (ch == '\'')
			{
				goto quit;
			}
			else if (ch == '\\')
			{
				char next = lexer_getch(lexer);
				ch = get_escape(next);
				if (ch == next)
				{
					lexer_warning(lexer, "未知的转义字符");
					lexer_ungetch(lexer);
					ch = '\\';
				}
			}
			break;
		}
		case S_ASSIGN:
		{
			lexer->token = TK_ASSIGN;
			if (ch == '=')
			{
				lexer->token = TK_EQ;
				goto storeandquit;
			}
			else
			{
				lexer_ungetch(lexer);
				goto quit;
			}
			break;
		}
		case S_EXCLA:
		{
			if (ch == '=')
			{
				lexer->token = TK_NEQ;
				goto storeandquit;
			}
			else
			{
				lexer_error(lexer, "暂不支持'!'");
			}
			break;
		}
		case S_GT:
		{
			lexer->token = TK_GT;
			if (ch == '=')
			{
				lexer->token = TK_GE;
				goto storeandquit;
			}
			else if (ch == '>')
			{
				lexer->token = TK_RSHIFT;
				goto storeandquit;
			}
			else
			{
				lexer_ungetch(lexer);
				goto quit;
			}
			break;
		}
		case S_LT:
		{
			lexer->token = TK_LT;
			if (ch == '=')
			{
				lexer->token = TK_LE;
				goto storeandquit;
			}
			else if (ch == '<')
			{
				lexer->token = TK_LSHIFT;
				goto storeandquit;
			}
			else
			{
				lexer_ungetch(lexer);
				goto quit;
			}
			break;
		}
		case S_PRE:
		{
			while (isspace(ch))ch = lexer_getch(lexer);
			char* dir = (char*)malloc(sizeof(char) * 1024);
			int dirlen = 0;
			while ((isdigit(ch) || isalpha(ch) || ch == '_'))
			{
				dir[dirlen++] = ch;
				dir[dirlen] = '\0';
				ch = lexer_getch(lexer);
			}
			if (!strcmp(dir, "enterfile")||!strcmp(dir,"backfile"))
			{
				while(isspace(ch))ch = lexer_getch(lexer);
				dirlen = 0;
				while (ch != '\n' && ch != EOF)
				{
					dir[dirlen++] = ch;
					dir[dirlen] = '\0';
					ch = lexer_getch(lexer);
				}
				lexer->filename = dir;
			}
			state = S_START;
			goto notstore;
			break;
		}
		case S_ELLIPSIS1:
			if (ch == '.')
			{
				state = S_ELLIPSIS2;
			}
			else
			{
				lexer->token = TK_POINT;
				lexer_ungetch(lexer);
				goto quit;
			}
			break;
		case S_ELLIPSIS2:
			if (ch == '.')
			{
				lexer->token = TK_ELLIPSIS;
				goto storeandquit;
			}
			else
			{
				lexer_error(lexer, "Unkown token");
			}
			break;
		}

#define STORE if (p_tokenstr >= max_tokenstr_len-1)\
		{\
			max_tokenstr_len += 1024;\
			char* old = lexer->tokenstr;\
			lexer->tokenstr = (char*)malloc(sizeof(char) * max_tokenstr_len);\
			strcpy(lexer->tokenstr, old);\
		}\
		lexer->tokenstr[p_tokenstr++] = ch;\
		lexer->tokenstr[p_tokenstr] = '\0';

		STORE;
	notstore:
		continue;
	storeandquit:
		STORE;
	quit:
		break;
#undef STORE
	}
}

void lexer_error(struct Lexer* lexer, char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	lexer_error2(lexer, format, arg);
	va_end(arg);
	exit(1);
}

void lexer_error2(struct Lexer* lexer, char* format, va_list arg)
{
	printf("Error:%s:%d:%d:", lexer->filename, lexer->line, lexer->column);
	vfprintf(stdout, format, arg);
	printf(":\n");
	if (lexer->linecode[strlen(lexer->linecode) - 1] == '\n')
	{
		printf("%s", lexer->linecode);
	}
	else
	{
		printf("%s\n", lexer->linecode);
	}
	int i = 0;
	for (; i < lexer->start_column - 1; i++)
	{
		if (isspace(lexer->linecode[i]))
		{
			printf("%c", lexer->linecode[i]);
		}
		else
		{
			printf(" ");
		}
	}
	for (; i < lexer->column; i++)
	{
		printf("^");
	}
	printf("\n");
	exit(1);
}

void lexer_warning(struct Lexer* lexer, char* format, ...)
{
	printf("Warning:%s:%d:%d:", lexer->filename, lexer->line, lexer->column);
	va_list arg;
	va_start(arg, format);
	vfprintf(stdout, format, arg);
	va_end(arg);
	printf(":\n");
	if (lexer->linecode[strlen(lexer->linecode) - 1] == '\n')
	{
		printf("%s", lexer->linecode);
	}
	else
	{
		printf("%s\n", lexer->linecode);
	}
	int i = 0;
	for (; i < lexer->start_column - 1; i++)
	{
		if (isspace(lexer->linecode[i]))
		{
			printf("%c", lexer->linecode[i]);
		}
		else
		{
			printf(" ");
		}
	}
	for (; i < lexer->column; i++)
	{
		printf("^");
	}
	printf("\n");
}

char get_escape(char ch)
{
	switch (ch)
	{
	case 'n':
		ch = '\n';
		break;
	case 't':
		ch = '\t';
		break;
	case 'r':
		ch = '\r';
		break;
	case '\'':
		ch = '\'';
		break;
	case '0':
		ch = '\0';
		break;
	case '"':
		ch = '"';
		break;
	}
	return ch;
}

char* get_tokenstr(enum TokenType token)
{
	for (int i = 0; i < TOKENSTRMAP_LEN; i++)
	{
		if (tokenstrmap[i].token == token)
		{
			return tokenstrmap[i].str;
		}
	}
	return "";
}