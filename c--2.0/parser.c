#include "Include/parser.h"
#include "Include/error.h"

struct Parser* parser_init(struct Lexer* lexer)
{
	struct Parser* parser = (struct Parser*)malloc(sizeof(struct Parser));
	if (NULL == parser)
	{
		printf("Parser初始化失败");
		exit(-1);
	}
	parser->symbol = symbol_init();
	parser->lexer = lexer;
	return parser;
}

void parser_match(struct Parser* parser, enum Token expect)
{
	if (parser->lexer->token != expect)
	{
		error("意料之外的token: '%d',应该为'%d'", parser->lexer->filename, parser->lexer->lineno, parser->lexer->linepos, parser->lexer->token, expect);
	}
	gettoken(parser->lexer);
}

struct SyntaxTree* parser_parse(struct Parser* parser)
{
	getch(parser->lexer);
	gettoken(parser->lexer);
	struct SyntaxTree* syntaxtree = syntaxtree_init(NT_PROGRAM, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	syntaxtree->child[NP_BODY] = parser_body(parser, "");
	return syntaxtree;
}

struct SyntaxTree* parser_body(struct Parser* parser, char* indent)
{
	struct SyntaxTree* t = syntaxtree_init(NT_NULL, AT_LIST, parser->lexer->lineno, parser->lexer->linepos);
	if (blockstyle == INDENT)
	{
		/*
		body::=(sentence)*}
		*/
		while (parser->lexer->token != TK_EOF && !strcmp(indent, parser->lexer->indent))
		{
			syntaxtreelist_add(t->list, parser_sentence(parser));
		}
	}
	else if (blockstyle == BEGINEND)
	{
		/*
		body::='begin' (sentence)* 'end'
		*/
		while (parser->lexer->token != TK_EOF && parser->lexer->token != TK_END)
		{
			syntaxtreelist_add(t->list, parser_sentence(parser));
		}
	}
	else if (blockstyle == BRACES)
	{
		/*
		body::='{' (sentence)* '}'
		*/
		while (parser->lexer->token != TK_EOF && parser->lexer->token != TK_RLARGE)
		{
			syntaxtreelist_add(t->list, parser_sentence(parser));
		}
	}
	return t;
}

struct SyntaxTree* parser_sentence(struct Parser* parser)
{
	struct SyntaxTree* t = NULL;
	if (parser->lexer->token == TK_IF)
	{
		t = parser_if(parser);
	}
	else if (parser->lexer->token == TK_WHILE)
	{
		t = parser_while(parser);
	}
	else if (parser->lexer->token == TK_NAME)
	{
		t = parser_assign(parser);
	}
	else if (parser->lexer->token == TK_INT)
	{
		t = parser_int(parser, 0);
	}
	else if (parser->lexer->token == TK_CHAR)
	{
		t = parser_char(parser, 0);
	}
	else if (parser->lexer->token == TK_VOID)
	{
		t = parser_void(parser, 0);
	}
	else if (parser->lexer->token == TK_SEMI)
	{
		while (parser->lexer->token == TK_SEMI)
		{
			parser_match(parser, TK_SEMI);
		}
	}
	else if (parser->lexer->token == TK_RETURN)
	{
		t = parser_return(parser);
	}
	else if (parser->lexer->token == TK_BREAK)
	{
		t = parser_break(parser);
	}
	else if (parser->lexer->token == TK_CONTINUE)
	{
		t = parser_continue(parser);
	}
	else if (parser->lexer->token == TK_LLARGE && blockstyle == BRACES)
	{
		parser_match(parser, TK_LLARGE);
		t = parser_body(parser, "\0");
	}
	else if (parser->lexer->token == TK_BEGIN && blockstyle == BEGINEND)
	{
		parser_match(parser, TK_BEGIN);
		t = parser_body(parser, "\0");
	}
	else if (parser->lexer->token == TK_STRUCT)
	{
		t = parser_struct(parser);
		if (blockstyle == BRACES)
		{
			parser_match(parser, TK_SEMI);
		}
	}
	else
	{
		error("'%d'token不能作为语句", parser->lexer->filename, parser->lexer->lineno, parser->lexer->linepos, parser->lexer->token);
	}
	return t;
}

struct SyntaxTree* parser_if(struct Parser* parser)
{
	struct SyntaxTree* t = syntaxtree_init(NT_IF, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	parser_match(parser, TK_IF);
	t->child[NP_EXP] = parser_exp(parser);
	if (blockstyle == INDENT)
	{
		/*
		if::='if' exp ':' body 'else' ':' body
		*/
		parser_match(parser, TK_COLON);
		t->child[NP_BODY] = parser_body(parser, parser->lexer->indent);
		if (parser->lexer->token == TK_ELSE)
		{
			parser_match(parser, TK_ELSE);
			parser_match(parser, TK_COLON);
			t->child[NP_ELSES] = parser_body(parser, parser->lexer->indent);
		}
	}
	else if (blockstyle == BEGINEND)
	{
		/*
		if::='if' exp 'begin' body 'end' 'else' 'begin' body 'end'
		*/
		t->child[NP_BODY] = parser_sentence(parser);
		if (parser->lexer->token == TK_ELSE)
		{
			parser_match(parser, TK_ELSE);
			t->child[NP_ELSES] = parser_sentence(parser);
		}
	}
	else if (blockstyle == BRACES)
	{
		/*
		if::='if' exp '{' body '}' 'else' '{' body '}'
		*/
		t->child[NP_BODY] = parser_sentence(parser);
		if (parser->lexer->token == TK_ELSE)
		{
			parser_match(parser, TK_ELSE);
			t->child[NP_ELSES] = parser_sentence(parser);
		}
	}
	return t;
}

struct SyntaxTree* parser_while(struct Parser* parser)
{
	struct SyntaxTree* t = syntaxtree_init(NT_WHILE, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	parser_match(parser, TK_WHILE);
	t->child[NP_EXP] = parser_exp(parser);
	if (blockstyle == INDENT)
	{
		parser_match(parser, TK_COLON);
		t->child[NP_BODY] = parser_body(parser, parser->lexer->indent);
	}
	else if (blockstyle == BEGINEND)
	{
		parser_match(parser, TK_BEGIN);
		t->child[NP_BODY] = parser_body(parser, "\0");
		parser_match(parser, TK_END);
	}
	else if (blockstyle == BRACES)
	{
		parser_match(parser, TK_LLARGE);
		t->child[NP_BODY] = parser_body(parser, "\0");
		parser_match(parser, TK_RLARGE);
	}
	return t;
}

struct SyntaxTree* parser_exp(struct Parser* parser)
{
	return parser_exp_or(parser);
}

struct SyntaxTree* parser_exp_or(struct Parser* parser)
{
	struct SyntaxTree* t = parser_exp_and(parser);
	while (parser->lexer->token == TK_OR)
	{
		parser_match(parser, TK_OR);
		struct SyntaxTree* p = syntaxtree_init(NT_OR, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		p->child[NP_LEFT] = t;
		p->child[NP_RIGHT] = parser_exp_and(parser);
		t = p;
	}
	return t;
}

struct SyntaxTree* parser_exp_and(struct Parser* parser)
{
	struct SyntaxTree* t = parser_exp_compare(parser);
	while (parser->lexer->token == TK_AND)
	{
		parser_match(parser, TK_AND);
		struct SyntaxTree* p = syntaxtree_init(NT_AND, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		p->child[NP_LEFT] = t;
		p->child[NP_RIGHT] = parser_exp_compare(parser);
		t = p;
	}
	return t;
}

struct SyntaxTree* parser_exp_compare(struct Parser* parser)
{
	struct SyntaxTree* t = parser_exp_shift(parser);
	if (parser->lexer->token == TK_GT || parser->lexer->token == TK_GEQ || parser->lexer->token == TK_LT ||
		parser->lexer->token == TK_LEQ || parser->lexer->token == TK_EQ || parser->lexer->token == TK_NEQ)
	{
		struct SyntaxTree* p = NULL;
		if (parser->lexer->token == TK_GT)
		{
			p = syntaxtree_init(NT_GT, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		}
		else if (parser->lexer->token == TK_GEQ)
		{
			p = syntaxtree_init(NT_GEQ, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		}
		else if (parser->lexer->token == TK_LT)
		{
			p = syntaxtree_init(NT_LT, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		}
		else if (parser->lexer->token == TK_LEQ)
		{
			p = syntaxtree_init(NT_LEQ, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		}
		else if (parser->lexer->token == TK_EQ)
		{
			p = syntaxtree_init(NT_EQ, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		}
		else if (parser->lexer->token == TK_NEQ)
		{
			p = syntaxtree_init(NT_NEQ, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		}
		parser_match(parser, parser->lexer->token);
		p->child[NP_LEFT] = t;
		p->child[NP_RIGHT] = parser_exp_shift(parser);
		t = p;
	}
	return t;
}

struct SyntaxTree* parser_exp_shift(struct Parser* parser)
{
	struct SyntaxTree* t = parser_exp_add(parser);
	while (parser->lexer->token == TK_LSHIFT || parser->lexer->token == TK_RSHIFT)
	{
		struct SyntaxTree* p = NULL;
		if (parser->lexer->token == TK_LSHIFT)
		{
			p = syntaxtree_init(NT_LSHIFT, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		}
		else if (parser->lexer->token == TK_LSHIFT)
		{
			p = syntaxtree_init(NT_LSHIFT, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		}
		parser_match(parser, parser->lexer->token);
		p->child[NP_LEFT] = t;
		p->child[NP_RIGHT] = parser_exp_add(parser);
		t = p;
	}
	return t;
}

struct SyntaxTree* parser_exp_add(struct Parser* parser)
{
	struct SyntaxTree* t = parser_exp_mul(parser);
	while (parser->lexer->token == TK_ADD || parser->lexer->token == TK_SUB)
	{
		struct SyntaxTree* p = NULL;
		if (parser->lexer->token == TK_ADD)
		{
			p = syntaxtree_init(NT_ADD, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		}
		else if (parser->lexer->token == TK_SUB)
		{
			p = syntaxtree_init(NT_SUB, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		}
		parser_match(parser, parser->lexer->token);
		p->child[NP_LEFT] = t;
		p->child[NP_RIGHT] = parser_exp_mul(parser);
		t = p;
	}
	return t;
}

struct SyntaxTree* parser_exp_mul(struct Parser* parser)
{
	struct SyntaxTree* t = parser_exp_primary(parser);
	while (parser->lexer->token == TK_MUL || parser->lexer->token == TK_DIV || parser->lexer->token == TK_MOD)
	{
		struct SyntaxTree* p = NULL;
		if (parser->lexer->token == TK_MUL)
		{
			p = syntaxtree_init(NT_MUL, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		}
		else if (parser->lexer->token == TK_DIV)
		{
			p = syntaxtree_init(NT_DIV, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		}
		else if (parser->lexer->token == TK_MOD)
		{
			p = syntaxtree_init(NT_MOD, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		}
		parser_match(parser, parser->lexer->token);
		p->child[NP_LEFT] = t;
		p->child[NP_RIGHT] = parser_exp_primary(parser);
		t = p;
	}
	return t;
}

struct SyntaxTree* parser_exp_primary(struct Parser* parser)
{
	struct SyntaxTree* t = NULL;
	if (parser->lexer->token == TK_NAME)
	{
		t = syntaxtree_init(NT_NAME, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		t->child[NP_ID] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		t->child[NP_ID]->string = parser->lexer->tokenstr;
		parser_match(parser, parser->lexer->token);
		if (parser->lexer->token == TK_LLITTLE)
		{
			struct SyntaxTree* p = syntaxtree_init(NT_CALL, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
			p->child[NP_TARGET] = t;
			p->child[NP_ARGS] = parser_args(parser);
			t = p;
		}
		else if (parser->lexer->token == TK_LMIDDLE)
		{
			while (parser->lexer->token == TK_LMIDDLE)
			{
				parser_match(parser, TK_LMIDDLE);
				struct SyntaxTree* p = syntaxtree_init(NT_INDEX, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
				p->child[NP_TARGET] = t;
				p->child[NP_EXP] = parser_exp(parser);
				t = p;
				parser_match(parser, TK_RMIDDLE);
			}
		}
		while (parser->lexer->token == TK_POINT)
		{
			parser_match(parser, TK_POINT);
			struct SyntaxTree* p = syntaxtree_init(NT_ATTR, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
			p->child[NP_VALUE] = t;
			p->child[NP_ATTR] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
			p->child[NP_ATTR]->string = parser->lexer->tokenstr;
			parser_match(parser, parser->lexer->token);
			t = p;
			if (parser->lexer->token == TK_LLITTLE)
			{
				struct SyntaxTree* p = syntaxtree_init(NT_CALL, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
				p->child[NP_TARGET] = t;
				p->child[NP_ARGS] = parser_args(parser);
				t = p;
			}
			else if (parser->lexer->token == TK_LMIDDLE)
			{
				while (parser->lexer->token == TK_LMIDDLE)
				{
					parser_match(parser, TK_LMIDDLE);
					struct SyntaxTree* p = syntaxtree_init(NT_INDEX, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
					p->child[NP_TARGET] = t;
					p->child[NP_EXP] = parser_exp(parser);
					t = p;
					parser_match(parser, TK_RMIDDLE);
				}
			}
		}
	}
	else if (parser->lexer->token == TK_NUM)
	{
		t = syntaxtree_init(NT_NUM, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		t->child[NP_VALUE] = syntaxtree_init(NT_NULL, AT_INT, parser->lexer->lineno, parser->lexer->linepos);
		t->child[NP_VALUE]->value = atoi(parser->lexer->tokenstr);
		parser_match(parser, parser->lexer->token);
	}
	else if (parser->lexer->token == TK_LETTER)
	{
		t = syntaxtree_init(NT_CHAR, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		t->child[NP_ID] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		t->child[NP_ID]->string = parser->lexer->tokenstr;
		parser_match(parser, parser->lexer->token);
	}
	else if (parser->lexer->token == TK_STRING)
	{
		t = syntaxtree_init(NT_STRING, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		t->child[NP_ID] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		t->child[NP_ID]->string = parser->lexer->tokenstr;
		symbol_addval(parser->symbol, t->child[NP_ID]->string, T_STRING);
		parser_match(parser, parser->lexer->token);
	}
	else if (parser->lexer->token == TK_LLITTLE)
	{
		parser_match(parser, TK_LLITTLE);
		if (parser->lexer->token != TK_RLITTLE)
		{
			t = parser_exp(parser);
		}
		parser_match(parser, TK_RLITTLE);
	}
	else if (parser->lexer->token == TK_INT)
	{
		t = parser_int(parser, 1);
	}
	else if (parser->lexer->token == TK_CHAR)
	{
		t = parser_char(parser, 1);
	}
	else if (parser->lexer->token == TK_VOID)
	{
		t = parser_void(parser, 1);
	}
	else
	{
		error("'%d'token不能作为表达式", parser->lexer->filename, parser->lexer->lineno, parser->lexer->linepos, parser->lexer->token);
	}
	return t;
}

struct SyntaxTree* parser_args(struct Parser* parser)
{
	parser_match(parser, TK_LLITTLE);
	struct SyntaxTree* t = syntaxtree_init(NT_NULL, AT_LIST, parser->lexer->lineno, parser->lexer->linepos);
	if (parser->lexer->token != TK_RLITTLE)
	{
		syntaxtreelist_add(t->list, parser_exp(parser));
		while (parser->lexer->token == TK_COMMA)
		{
			parser_match(parser, TK_COMMA);
			syntaxtreelist_add(t->list, parser_exp(parser));
		}
	}
	parser_match(parser, TK_RLITTLE);
	return t;
}

struct SyntaxTree* parser_assign(struct Parser* parser)
{
	struct SyntaxTree* t = syntaxtree_init(NT_NAME, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	t->child[NP_ID] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
	t->child[NP_ID]->string = parser->lexer->tokenstr;
	parser_match(parser, parser->lexer->token);
	/*用户自定义变量声明或实现*/
	if (parser->lexer->token == TK_NAME)
	{
		symbol_addstruct(parser->symbol, parser->lexer->tokenstr, t->child[NP_ID]->string);
		return parser_define(parser, 1, T_STRUCT, t->child[NP_ID]->string);
	}
	if (parser->lexer->token == TK_LLITTLE)
	{
		struct SyntaxTree* p = syntaxtree_init(NT_CALL, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		p->child[NP_TARGET] = t;
		p->child[NP_ARGS] = parser_args(parser);
		t = p;
	}
	else if (parser->lexer->token == TK_LMIDDLE)
	{
		while (parser->lexer->token == TK_LMIDDLE)
		{
			parser_match(parser, TK_LMIDDLE);
			struct SyntaxTree* p = syntaxtree_init(NT_INDEX, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
			p->child[NP_TARGET] = t;
			p->child[NP_EXP] = parser_exp(parser);
			t = p;
			parser_match(parser, TK_RMIDDLE);
		}
	}
	while (parser->lexer->token == TK_POINT)
	{
		parser_match(parser, TK_POINT);
		struct SyntaxTree* p = syntaxtree_init(NT_ATTR, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		p->child[NP_VALUE] = t;
		p->child[NP_ATTR] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		p->child[NP_ATTR]->string = parser->lexer->tokenstr;
		parser_match(parser, parser->lexer->token);
		t = p;
		if (parser->lexer->token == TK_LLITTLE)
		{
			struct SyntaxTree* p = syntaxtree_init(NT_CALL, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
			p->child[NP_TARGET] = t;
			p->child[NP_ARGS] = parser_args(parser);
			t = p;
		}
		else if (parser->lexer->token == TK_LMIDDLE)
		{
			while (parser->lexer->token == TK_LMIDDLE)
			{
				parser_match(parser, TK_LMIDDLE);
				struct SyntaxTree* p = syntaxtree_init(NT_INDEX, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
				p->child[NP_TARGET] = t;
				p->child[NP_EXP] = parser_exp(parser);
				t = p;
				parser_match(parser, TK_RMIDDLE);
			}
		}
	}
	if (parser->lexer->token == TK_ASSIGN)
	{
		parser_match(parser, TK_ASSIGN);
		struct SyntaxTree* p = syntaxtree_init(NT_ASSIGN, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		p->child[NP_TARGET] = t;
		p->child[NP_EXP] = parser_exp(parser);
		t = p;
	}
	if (blockstyle != INDENT)
	{
		parser_match(parser, TK_SEMI);
	}
	return t;
}

struct SyntaxTree* parser_break(struct Parser* parser)
{
	struct SyntaxTree* t = syntaxtree_init(NT_BREAK, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	parser_match(parser, TK_BREAK);
	return t;
}

struct SyntaxTree* parser_continue(struct Parser* parser)
{
	struct SyntaxTree* t = syntaxtree_init(NT_CONTINUE, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	parser_match(parser, TK_CONTINUE);
	return t;
}

struct SyntaxTree* parser_return(struct Parser* parser)
{
	struct SyntaxTree* t = syntaxtree_init(NT_RETURN, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	parser_match(parser, TK_RETURN);
	t->child[NP_EXP] = parser_exp(parser);
	return t;
}

struct SyntaxTree* parser_int(struct Parser* parser, int exp)
{
	parser_match(parser, TK_INT);
	return parser_define(parser, exp, T_INT, "int");
}

struct SyntaxTree* parser_char(struct Parser* parser, int exp)
{
	parser_match(parser, TK_CHAR);
	return parser_define(parser, exp, T_CHAR, "char");
}

struct SyntaxTree* parser_void(struct Parser* parser, int exp)
{
	parser_match(parser, TK_VOID);
	return parser_define(parser, exp, T_VOID, "void");
}

struct SyntaxTree* parser_struct(struct Parser* parser)
{
	struct SyntaxTree* t = syntaxtree_init(NT_DEFINE, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	parser_match(parser, TK_STRUCT);
	t->child[NP_ID] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
	t->child[NP_ID]->string = parser->lexer->tokenstr;
	parser_match(parser, parser->lexer->token);
	symbol_addval(parser->symbol, t->child[NP_ID]->string, T_STRUCT);
	if (blockstyle == INDENT)
	{
		parser_match(parser, TK_COLON);
		t->child[NP_BODY] = parser_body(parser, parser->lexer->indent);
	}
	else if (blockstyle == BEGINEND)
	{
		parser_match(parser, TK_BEGIN);
		t->child[NP_BODY] = parser_body(parser, parser->lexer->indent);
		parser_match(parser, TK_END);
	}
	else if (blockstyle == BRACES)
	{
		parser_match(parser, TK_LLARGE);
		t->child[NP_BODY] = parser_body(parser, parser->lexer->indent);
		parser_match(parser, TK_RLARGE);
	}
	return t;
}

struct SyntaxTree* parser_define(struct Parser* parser, int exp, enum Type type, char* other_type)
{
	struct SyntaxTree* t = syntaxtree_init(NT_DEFINE, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	/*变量声明*/
	t->child[NP_ID] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
	t->child[NP_ID]->string = parser->lexer->tokenstr;
	parser_match(parser, parser->lexer->token);
	/*函数声明*/
	if (parser->lexer->token == TK_LLITTLE)
	{
		struct SyntaxTree* args = parser_args(parser);
		symbol_addfunc(parser->symbol, t->child[NP_ID]->string, other_type,args);
		/*函数实现*/
		if (parser->lexer->token == TK_COLON && blockstyle == INDENT)
		{
			parser_match(parser, TK_COLON);
			struct SyntaxTree* func = syntaxtree_init(NT_FUNCTION, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
			func->child[NP_ID] = t->child[NP_ID];
			func->child[NP_ARGS] = args;
			func->child[NP_BODY] = parser_body(parser, parser->lexer->indent);
			t = func;
		}
		else if (parser->lexer->token == TK_BEGIN && blockstyle == BEGINEND)
		{
			parser_match(parser, TK_BEGIN);
			struct SyntaxTree* func = syntaxtree_init(NT_FUNCTION, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
			func->child[NP_ID] = t->child[NP_ID];
			func->child[NP_ARGS] = args;
			func->child[NP_BODY] = parser_body(parser, "\0");
			parser_match(parser, TK_END);
			t = func;
		}
		else if (parser->lexer->token == TK_LLARGE && blockstyle == BRACES)
		{
			parser_match(parser, TK_LLARGE);
			struct SyntaxTree* func = syntaxtree_init(NT_FUNCTION, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
			func->child[NP_ID] = t->child[NP_ID];
			func->child[NP_ARGS] = args;
			func->child[NP_BODY] = parser_body(parser, "\0");
			parser_match(parser, TK_RLARGE);
			t = func;
		}
		else
		{
			if (blockstyle != INDENT && !exp)
			{
				parser_match(parser, TK_SEMI);
			}
		}
	}
	else if (parser->lexer->token == TK_LMIDDLE)
	{
		/*数组声明*/
		struct SyntaxTree* p = syntaxtree_init(NT_NULL, AT_LIST, parser->lexer->lineno, parser->lexer->linepos);
		symbol_addarray(parser->symbol, t->child[NP_ID]->string, other_type);
		while (parser->lexer->token == TK_LMIDDLE)
		{
			parser_match(parser, TK_LMIDDLE);
			syntaxtreelist_add(p->list, parser_exp(parser));
			parser_match(parser, TK_RMIDDLE);
		}
		if (blockstyle != INDENT && !exp)
		{
			parser_match(parser, TK_SEMI);
		}
	}
	else if (parser->lexer->token == TK_ASSIGN)
	{
		/*初始值*/
		parser_match(parser, TK_ASSIGN);
		t->child[NP_DEFAULT] = parser_exp(parser);
		symbol_addval(parser->symbol, t->child[NP_ID]->string, type);
		if (blockstyle != INDENT && !exp)
		{
			parser_match(parser, TK_SEMI);
		}
	}
	else
	{
		symbol_addval(parser->symbol, t->child[NP_ID]->string, type);
		if (blockstyle != INDENT && !exp)
		{
			parser_match(parser, TK_SEMI);
		}
	}
	return t;
}