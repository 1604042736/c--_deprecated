#include "Include/lexer.h"

Lexer* lexer_init(string content,string filename)
{
	Lexer* lexer = new Lexer;
	lexer->content = content;
	lexer->filename = filename;
	lexer->lineno = 1;
	lexer->linepos = 0;
	lexer->pos = 0;
	lexer->eof = false;
	return lexer;
}

void lexer_getch(Lexer* lexer)
{
	if (lexer->pos < lexer->content.length())
	{
		if (lexer->content[lexer->pos] == '\n')
		{
			lexer->lineno++;
			lexer->linepos = 0;
		}
		lexer->linepos++;
		lexer->ch = lexer->content[lexer->pos++];
	}
	else
	{
		lexer->eof = true;
		lexer->ch = EOF;
	}
}

void lexer_ungetch(Lexer* lexer)
{
	if (!lexer->eof)
	{
		lexer->pos--;
		lexer->linepos--;
	}
}

void lexer_gettoken(Lexer* lexer)
{
	//TODO 缩进
	while (lexer->ch != EOF && (lexer->ch == ' ' || lexer->ch == '\n' || lexer->ch == '\t'))
	{
		if (lexer->ch == '\n' && lexer->blockstyle == INDENT)
		{
			lexer_getindent(lexer);
		}
		else
		{
			lexer_getch(lexer);
		}
	}
	if (isName(lexer->ch))
	{
		lexer_getname(lexer);
	}
	else if (isNum(lexer->ch))
	{
		lexer_getnum(lexer);
	}
	else if (lexer->ch == '{')
	{
		lexer->token = TK_LLARGE;
		lexer_getch(lexer);
	}
	else if (lexer->ch == '}')
	{
		lexer->token = TK_RLARGE;
		lexer_getch(lexer);
	}
	else if (lexer->ch == '<')
	{
		lexer_getch(lexer);
		if (lexer->ch == '=')
		{
			lexer->token = TK_LEQ;
			lexer->tokenstr = "<=";
			lexer_getch(lexer);
		}
		else if (lexer->ch == '<')
		{
			lexer->token = TK_LSHIFT;
			lexer->tokenstr = "<<";
			lexer_getch(lexer);
		}
		else
		{
			lexer->token = TK_LT;
			lexer->tokenstr = "<";
		}
	}
	else if (lexer->ch == '>')
	{
		lexer_getch(lexer);
		if (lexer->ch == '=')
		{
			lexer->token = TK_GEQ;
			lexer->tokenstr = ">=";
			lexer_getch(lexer);
		}
		else if (lexer->ch == '>')
		{
			lexer->token = TK_RSHIFT;
			lexer->tokenstr = ">>";
			lexer_getch(lexer);
		}
		else
		{
			lexer->token = TK_GT;
			lexer->tokenstr = ">";
		}
	}
	else if (lexer->ch == '=')
	{
		lexer_getch(lexer);
		if (lexer->ch == '=')
		{
			lexer->token = TK_EQ;
			lexer->tokenstr = "==";
			lexer_getch(lexer);
		}
		else
		{
			lexer->token = TK_ASSIGN;
			lexer->tokenstr = "=";
		}
	}
	else if (lexer->ch == '!')
	{
		lexer_getch(lexer);
		if (lexer->ch == '=')
		{
			lexer->token = TK_NEQ;
			lexer->tokenstr = "!=";
			lexer_getch(lexer);
		}
		else
		{
			//TODO 错误提示
		}
	}
	else if (lexer->ch == '+')
	{
		lexer->token = TK_ADD;
		lexer->tokenstr = "+";
		lexer_getch(lexer);
	}
	else if (lexer->ch == '-')
	{
		lexer->token = TK_SUB;
		lexer->tokenstr = "-";
		lexer_getch(lexer);
	}
	else if (lexer->ch == '*')
	{
		lexer->token = TK_MUL;
		lexer->tokenstr = "*";
		lexer_getch(lexer);
	}
	else if (lexer->ch == '/')
	{
		lexer->token = TK_DIV;
		lexer->tokenstr = "/";
		lexer_getch(lexer);
	}
	else if (lexer->ch == '%')
	{
		lexer->token = TK_MOD;
		lexer->tokenstr = "%";
		lexer_getch(lexer);
	}
	else if (lexer->ch == ',')
	{
		lexer->token = TK_COMMA;
		lexer->tokenstr = ",";
		lexer_getch(lexer);
	}
	else if (lexer->ch == ';')
	{
		lexer->token = TK_SEMI;
		lexer->tokenstr = ";";
		lexer_getch(lexer);
	}
	else if (lexer->ch == ':')
	{
		lexer->token = TK_COLON;
		lexer->tokenstr = ":";
		lexer_getch(lexer);
	}
	else if (lexer->ch == '.')
	{
		lexer->token = TK_POINT;
		lexer->tokenstr = ".";
		lexer_getch(lexer);
	}
	else if (lexer->ch == '(')
	{
		lexer->token = TK_LLITTLE;
		lexer->tokenstr = "(";
		lexer_getch(lexer);
	}
	else if (lexer->ch == ')')
	{
		lexer->token = TK_RLITTLE;
		lexer->tokenstr = ")";
		lexer_getch(lexer);
	}
	else if (lexer->ch == '[')
	{
		lexer->token = TK_LMIDDLE;
		lexer->tokenstr = "[";
		lexer_getch(lexer);
	}
	else if (lexer->ch == ']')
	{
		lexer->token = TK_RMIDDLE;
		lexer->tokenstr = "]";
		lexer_getch(lexer);
	}
	else if (lexer->ch == '"')
	{
		lexer_getstring(lexer, '"');
	}
	else if (lexer->ch == '\'')
	{
		lexer_getstring(lexer, '\'');
	}
	else if(lexer->ch==EOF)
	{
		lexer->token = TK_EOF;
	}
	else
	{
		cout << "未知的字符: " << lexer->ch;
		exit(-1);
	}
}

void lexer_getname(Lexer* lexer)
{
	lexer->tokenstr = "";
	while (isName(lexer->ch) || isNum(lexer->ch))
	{
		lexer->tokenstr += lexer->ch;
		lexer_getch(lexer);
	}
	for (int i = 0; i < MAXTOKENMAPLEN; i++)
	{
		if (tokenmap[i].str == lexer->tokenstr)
		{
			lexer->token = tokenmap[i].tk;
			return;
		}
	}
	lexer->token = TK_NAME;
}

void lexer_getnum(Lexer* lexer)
{
	lexer->tokenstr = "";
	while (isNum(lexer->ch))
	{
		lexer->tokenstr += lexer->ch;
		lexer_getch(lexer);
	}
	if (lexer->ch == '.')	//小数
	{
		lexer->tokenstr += lexer->ch;
		lexer_getch(lexer);
		while (isNum(lexer->ch))
		{
			lexer->tokenstr += lexer->ch;
			lexer_getch(lexer);
		}
	}
	lexer->token = TK_NUM;
}

void lexer_getindent(Lexer* lexer)
{
	lexer_getch(lexer);
	lexer->indent = "";
	while (lexer->ch != EOF && (lexer->ch == ' ' || lexer->ch == '\t'))
	{
		lexer->indent+=lexer->ch;
		lexer_getch(lexer);
	}
	/*如果是空行则重新获取缩进*/
	if (lexer->ch == '\n')
	{
		lexer_getindent(lexer);
	}
}

void lexer_getstring(Lexer* lexer, char end)
{
	lexer_getch(lexer);
	lexer->tokenstr = "";
	while (lexer->ch != EOF && lexer->ch != end)
	{
		if (lexer->ch == '\\')
		{
			lexer_getch(lexer);
			switch (lexer->ch)
			{
			case 'n':
				lexer->ch = '\n';
				break;
			case 't':
				lexer->ch = '\t';
				break;
			case 'r':
				lexer->ch = '\r';
				break;
			}
		}
		lexer->tokenstr += lexer->ch;
		lexer_getch(lexer);
	}
	lexer_getch(lexer);
	lexer->token = TK_STRING;
}