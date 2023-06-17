#include "parser.h"

struct Parser* parser_new(struct Lexer* lexer)
{
	struct Parser* parser = (struct Parser*)malloc(sizeof(struct Parser));
	if (parser == NULL)
	{
		printf("Parser分配内存失败");
		exit(1);
	}
	parser->lexer = lexer;
	return parser;
}

void parser_match(struct Parser* parser, enum TokenType tokentype)
{
	if (parser->lexer->token != tokentype)
	{
		parser_error(parser, "Expected '%s',but got '%s'", get_tokenstr(tokentype), get_tokenstr(parser->lexer->token));
	}
	lexer_gettoken(parser->lexer);
}

void parser_error(struct Parser* parser, char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	lexer_error2(parser->lexer, format, arg);
	va_end(arg);
}

struct AST* parser_start(struct Parser* parser)
{
	lexer_gettoken(parser->lexer);
	struct AST* ast = ast_new(NT_PROGRAM, parser);
	ast->child[0] = parser_statements(parser);
	return ast;
}

/*
type:
	| type "*"
	| type "[" NUMBER "]"
	| type "(" [type ["," type]* ["," "..."]] ")"
	| "int"
	| "char"
	| "struct" NAME
	| "(" type ")"
*/
struct Type* parser_type(struct Parser* parser)
{
	enum TokenType token = parser->lexer->token;
	struct Type* type = NULL;
	if (token == KW_INT)
	{
		type = type_new(T_INT);
		parser_match(parser, KW_INT);
	}
	else if (token == KW_CHAR)
	{
		type = type_new(T_CHAR);
		parser_match(parser, KW_CHAR);
	}
	else if (token == KW_VOID)
	{
		type = type_new(T_VOID);
		parser_match(parser, KW_VOID);
	}
	else if (token == KW_STRUCT)
	{
		type = type_new(T_STRUCT);
		parser_match(parser, KW_STRUCT);
		type->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, TK_NAME);
	}
	else if (token == TK_LLITTLE)
	{
		parser_match(parser, TK_LLITTLE);
		type = parser_type(parser);
		parser_match(parser, TK_RLITTLE);
	}
	token = parser->lexer->token;
	while (token == TK_STAR || token == TK_LMIDDLE || token == TK_LLITTLE)
	{
		if (token == TK_STAR)
		{
			struct Type* t = type_new(T_POINTER);
			t->child[0] = type;
			type = t;
			parser_match(parser, TK_STAR);
		}
		else if (token == TK_LMIDDLE)
		{
			struct Type* t = type_new(T_ARRAY);
			t->child[0] = type;
			parser_match(parser, TK_LMIDDLE);
			t->attr.ival = atoi(parser->lexer->tokenstr);
			parser_match(parser, TK_INT);
			parser_match(parser, TK_RMIDDLE);
			type = t;
		}
		else if (token == TK_LLITTLE)
		{
			struct Type* t = type_new(T_FUNC);
			t->child[0] = type;
			parser_match(parser, TK_LLITTLE);
			if (parser->lexer->token != TK_RLITTLE)
			{
				t->child[1] = parser_type(parser);
				struct Type* p = t->child[1];
				while (parser->lexer->token == TK_COMMA)
				{
					parser_match(parser, TK_COMMA);
					if (parser->lexer->token == TK_ELLIPSIS)
					{
						parser_match(parser, TK_ELLIPSIS);
						t->attribute |= TF_INDEFARG;
						break;
					}
					p->sibling = parser_type(parser);
					p = p->sibling;
				}
			}
			parser_match(parser, TK_RLITTLE);
			type = t;
		}
		token = parser->lexer->token;
	}
	return type;
}

/*
statements:
	| statement*
*/
struct AST* parser_statements(struct Parser* parser)
{
	struct AST* ast = parser_statement(parser);
	struct AST* p = ast;
	while (p != NULL && parser->lexer->token != TK_EOF && parser->lexer->token != TK_RLARGE)
	{
		struct AST* t = parser_statement(parser);
		p->sibling = t;
		p = t;
	}
	return ast;
}

/*
statement:
	| compoundstmt
	| ifstmt
	| funcdef
	| defstmt
	| returnstmt ";"
	| breakstmt ";"
	| continue ";"
	| structdef
	| exp ";"
*/
struct AST* parser_statement(struct Parser* parser)
{
	struct AST* ast = NULL;
	if (parser->lexer->token == TK_LLARGE)
	{
		ast = parser_compoundstmt(parser);
	}
	else if (parser->lexer->token == KW_IF)
	{
		ast = parser_ifstmt(parser);
	}
	else if (parser->lexer->token == KW_FUNC)
	{
		ast = parser_funcdef(parser);
	}
	else if (parser->lexer->token == KW_VAR)
	{
		ast = parser_defstmt(parser);
	}
	else if (parser->lexer->token == KW_RETURN)
	{
		ast = parser_returnstmt(parser);
		parser_match(parser, TK_SEMI);
	}
	else if (parser->lexer->token == KW_BREAK)
	{
		ast = parser_breakstmt(parser);
		parser_match(parser, TK_SEMI);
	}
	else if (parser->lexer->token == KW_CONTINUE)
	{
		ast = parser_continuestmt(parser);
		parser_match(parser, TK_SEMI);
	}
	else if (parser->lexer->token == KW_EXTERN)
	{
		ast = parser_externstmt(parser);
	}
	else if (parser->lexer->token == KW_WHILE)
	{
		ast = parser_whilestmt(parser);
	}
	else if (parser->lexer->token == KW_STRUCT)
	{
		ast = parser_structdef(parser);
	}
	else
	{
		ast = parser_exp(parser);
		parser_match(parser, TK_SEMI);
	}
	return ast;
}

/*
compoundstmt:
	| "{" statements "}"
*/
struct AST* parser_compoundstmt(struct Parser* parser)
{
	struct AST* ast = ast_new(NT_COMPOUNDSTMT, parser);
	parser_match(parser, TK_LLARGE);
	if (parser->lexer->token != TK_RLARGE)
	{
		ast->child[0] = parser_statements(parser);
	}
	parser_match(parser, TK_RLARGE);
	return ast;
}

/*
ifstmt:
	| "if" exp statement "else" statement
	| "if" exp statement
*/
struct AST* parser_ifstmt(struct Parser* parser)
{
	struct AST* ast = ast_new(NT_IFSTMT, parser);
	parser_match(parser, KW_IF);
	ast->child[0] = parser_exp(parser);
	ast->child[1] = parser_statement(parser);
	if (parser->lexer->token == KW_ELSE)
	{
		parser_match(parser, KW_ELSE);
		ast->child[2] = parser_statement(parser);
	}
	return ast;
}

/*
exp:
	| exp_assign
*/
struct AST* parser_exp(struct Parser* parser)
{
	return parser_exp_assign(parser);
}

/*
exp_assign:
	| exp_assign "=" exp
	| exp_or
*/
struct AST* parser_exp_assign(struct Parser* parser)
{
	struct AST* ast = parser_exp_or(parser);
	while (parser->lexer->token == TK_ASSIGN)
	{
		struct AST* t = ast_new(NT_BINOP, parser);
		t->child[0] = ast;
		t->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, TK_ASSIGN);
		t->child[1] = parser_exp(parser);
		ast = t;
	}
	return ast;
}

/*
exp_or:
	| exp_or "or" exp_and
	| exp_and
*/
struct AST* parser_exp_or(struct Parser* parser)
{
	struct AST* ast = parser_exp_and(parser);
	while (parser->lexer->token == KW_OR)
	{
		struct AST* t = ast_new(NT_BINOP, parser);
		t->child[0] = ast;
		t->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, KW_OR);
		t->child[1] = parser_exp_and(parser);
		ast = t;
	}
	return ast;
}

/*
exp_and:
	| exp_and "and" exp_bitor
	| exp_bitor
*/
struct AST* parser_exp_and(struct Parser* parser)
{
	struct AST* ast = parser_exp_bitor(parser);
	while (parser->lexer->token == KW_AND)
	{
		struct AST* t = ast_new(NT_BINOP, parser);
		t->child[0] = ast;
		t->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, KW_AND);
		t->child[1] = parser_exp_bitor(parser);
		ast = t;
	}
	return ast;
}

/*
exp_bitor:
	| exp_bitor "|" exp_bitxor
	| exp_bitxor
*/
struct AST* parser_exp_bitor(struct Parser* parser)
{
	struct AST* ast = parser_exp_bitxor(parser);
	while (parser->lexer->token == TK_BITOR)
	{
		struct AST* t = ast_new(NT_BINOP, parser);
		t->child[0] = ast;
		t->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, TK_BITOR);
		t->child[1] = parser_exp_bitxor(parser);
		ast = t;
	}
	return ast;
}

/*
exp_bitxor:
	| exp_bitxor "^" exp_bitand
	| exp_bitand
*/
struct AST* parser_exp_bitxor(struct Parser* parser)
{
	struct AST* ast = parser_exp_bitand(parser);
	while (parser->lexer->token == TK_BITXOR)
	{
		struct AST* t = ast_new(NT_BINOP, parser);
		t->child[0] = ast;
		t->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, TK_BITXOR);
		t->child[1] = parser_exp_bitand(parser);
		ast = t;
	}
	return ast;
}

/*
exp_bitand:
	| exp_bitand "&" exp_compare
	| exp_compare
*/
struct AST* parser_exp_bitand(struct Parser* parser)
{
	struct AST* ast = parser_exp_compare(parser);
	while (parser->lexer->token == TK_BITAND)
	{
		struct AST* t = ast_new(NT_BINOP, parser);
		t->child[0] = ast;
		t->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, TK_BITAND);
		t->child[1] = parser_exp_compare(parser);
		ast = t;
	}
	return ast;
}

/*
exp_compare:
	| exp_compare "==" exp_shift
	| exp_compare ">" exp_shift
	| exp_compare ">=" exp_shift
	| exp_compare "<" exp_shift
	| exp_compare "<=" exp_shift
	| exp_shift
*/
struct AST* parser_exp_compare(struct Parser* parser)
{
	struct AST* ast = parser_exp_shift(parser);
	enum TokenType token = parser->lexer->token;
	while (token == TK_EQ || token == TK_NEQ || token == TK_GT || token == TK_GE || token == TK_LT || token == TK_LE)
	{
		struct AST* t = ast_new(NT_BINOP, parser);
		t->child[0] = ast;
		t->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, token);
		t->child[1] = parser_exp_shift(parser);
		ast = t;
		token = parser->lexer->token;
	}
	return ast;
}

/*
exp_shift:
	| exp_shift ">>" exp_sum
	| exp_shift "<<" exp_sum
	| exp_sum
*/
struct AST* parser_exp_shift(struct Parser* parser)
{
	struct AST* ast = parser_exp_sum(parser);
	enum TokenType token = parser->lexer->token;
	while (token == TK_LSHIFT || token == TK_RSHIFT)
	{
		struct AST* t = ast_new(NT_BINOP, parser);
		t->child[0] = ast;
		t->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, token);
		t->child[1] = parser_exp_sum(parser);
		ast = t;
		token = parser->lexer->token;
	}
	return ast;
}

/*
exp_sum:
	| exp_sum "+" exp_term
	| exp_sum "-" exp_term
	| exp_term
*/
struct AST* parser_exp_sum(struct Parser* parser)
{
	struct AST* ast = parser_exp_term(parser);
	enum TokenType token = parser->lexer->token;
	while (token == TK_PLUS || token == TK_MINUS)
	{
		struct AST* t = ast_new(NT_BINOP, parser);
		t->child[0] = ast;
		t->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, token);
		t->child[1] = parser_exp_term(parser);
		ast = t;
		token = parser->lexer->token;
	}
	return ast;
}

/*
exp_term:
	| exp_term "*" exp_unary
	| exp_term "/" exp_unary
	| exp_term "%" exp_unary
	| exp_unary
*/
struct AST* parser_exp_term(struct Parser* parser)
{
	struct AST* ast = parser_exp_unary(parser);
	enum TokenType token = parser->lexer->token;
	while (token == TK_STAR || token == TK_DIV || token == TK_MOD)
	{
		struct AST* t = ast_new(NT_BINOP, parser);
		t->child[0] = ast;
		t->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, token);
		t->child[1] = parser_exp_unary(parser);
		ast = t;
		token = parser->lexer->token;
	}
	return ast;
}

/*
exp_unary:
	| "&" exp_unary
	| "*" exp_unary
	| "<" type ">" exp_unary
	| exp_primary
*/
struct AST* parser_exp_unary(struct Parser* parser)
{
	struct AST* ast = NULL;
	if (parser->lexer->token == TK_BITAND || parser->lexer->token == TK_STAR)
	{
		ast = ast_new(NT_UNARYOP, parser);
		ast->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, parser->lexer->token);
		ast->child[0] = parser_exp_unary(parser);
	}
	else if (parser->lexer->token == TK_LT)
	{
		ast = ast_new(NT_IMPLICITCAST, parser);
		parser_match(parser, TK_LT);
		ast->type = parser_type(parser);
		parser_match(parser, TK_GT);
		ast->child[0] = parser_exp_unary(parser);
	}
	else
	{
		ast = parser_exp_primary(parser);
	}
	return ast;
}

/*
exp_primary:
	| exp_primary "(" [exp ('," exp)+] ")"
	| exp_primary "[" exp "]"
	| exp_primary "." NAME
	| exp_atom
*/
struct AST* parser_exp_primary(struct Parser* parser)
{
	struct AST* ast = parser_exp_atom(parser);
	enum TokenType token = parser->lexer->token;
	while (token == TK_LLITTLE || token == TK_LMIDDLE || token == TK_POINT)
	{
		if (token == TK_LLITTLE)
		{
			struct AST* t = ast_new(NT_CALL, parser);
			t->child[0] = ast;
			ast = t;
			parser_match(parser, TK_LLITTLE);
			if (parser->lexer->token != TK_RLITTLE)
			{
				t = parser_exp(parser);
				ast->child[1] = t;
				while (parser->lexer->token == TK_COMMA)
				{
					struct AST* p = t;
					parser_match(parser, TK_COMMA);
					t->sibling = parser_exp(parser);
					t = t->sibling;
					t->last_sibling = p;
				}
			}
			parser_match(parser, TK_RLITTLE);
		}
		else if (token == TK_LMIDDLE)
		{
			struct AST* t = ast_new(NT_SUBSCRIPT, parser);
			t->child[0] = ast;
			parser_match(parser, TK_LMIDDLE);
			t->child[1] = parser_exp(parser);
			parser_match(parser, TK_RMIDDLE);
			ast = t;
		}
		else if (token == TK_POINT)
		{
			struct AST* t = ast_new(NT_MEMBER, parser);
			t->child[0] = ast;
			parser_match(parser, TK_POINT);
			t->attr.sval = parser->lexer->tokenstr;
			parser_match(parser, TK_NAME);
			ast = t;
		}
		token = parser->lexer->token;
	}
	return ast;
}

/*
exp_atom
	| NAME
	| STRING
	| CHAR
	| INT
	| DEC
	| "(" exp ")"
*/
struct AST* parser_exp_atom(struct Parser* parser)
{
	struct AST* ast = NULL;
	if (parser->lexer->token == TK_NAME)
	{
		ast = ast_new(NT_NAME, parser);
		ast->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, TK_NAME);
	}
	else if (parser->lexer->token == TK_STRING)
	{
		ast = ast_new(NT_STRING, parser);
		ast->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, TK_STRING);
	}
	else if (parser->lexer->token == TK_CHAR)
	{
		ast = ast_new(NT_CHAR, parser);
		ast->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, TK_CHAR);
	}
	else if (parser->lexer->token == TK_INT)
	{
		ast = ast_new(NT_INT, parser);
		ast->attr.ival = atoi(parser->lexer->tokenstr);
		parser_match(parser, TK_INT);
	}
	else if (parser->lexer->token == TK_DEC)
	{
		ast = ast_new(NT_DEC, parser);
		ast->attr.dval = atof(parser->lexer->tokenstr);
		parser_match(parser, TK_DEC);
	}
	else if (parser->lexer->token == TK_LLITTLE)
	{
		parser_match(parser, TK_LLITTLE);
		ast = parser_exp(parser);
		parser_match(parser, TK_RLITTLE);
	}
	return ast;
}


/*
defstmt:
	| "var" type NAME ["=" exp] ("," type NAME ["=" exp])* ";"
*/
struct AST* parser_defstmt(struct Parser* parser)
{
	struct AST* ast = ast_new(NT_DEFSTMT, parser);
	parser_match(parser, KW_VAR);

	struct AST* p = ast_new(NT_VARDEF, parser);
	p->type = parser_type(parser);
	p->attr.sval = parser->lexer->tokenstr;
	parser_match(parser, TK_NAME);
	if (parser->lexer->token == TK_ASSIGN)
	{
		parser_match(parser, TK_ASSIGN);
		p->child[0] = parser_exp(parser);
	}
	ast->child[0] = p;

	while (parser->lexer->token == TK_COMMA)
	{
		parser_match(parser, TK_COMMA);
		struct AST* t = ast_new(NT_VARDEF, parser);
		t->type = parser_type(parser);
		t->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, TK_NAME);
		if (parser->lexer->token == TK_ASSIGN)
		{
			parser_match(parser, TK_ASSIGN);
			t->child[0] = parser_exp(parser);
		}
		p->sibling = t;
		p = t;
	}
	parser_match(parser, TK_SEMI);
	return ast;
}

/*
funcdef:
	| "func" type NAME "(" [argsdef] ")" compoundstmt
	| "func" type NAME "(" [argsdef] ")" ";"
*/
struct AST* parser_funcdef(struct Parser* parser)
{
	struct AST* ast = ast_new(NT_FUNCDEF, parser);
	parser_match(parser, KW_FUNC);
	ast->type = parser_type(parser);
	ast->attr.sval = parser->lexer->tokenstr;
	parser_match(parser, TK_NAME);
	parser_match(parser, TK_LLITTLE);
	if (parser->lexer->token != TK_RLITTLE)
	{
		ast->child[0] = parser_argsdef(parser);
	}
	parser_match(parser, TK_RLITTLE);
	if (parser->lexer->token == TK_SEMI)
	{
		parser_match(parser, TK_SEMI);
	}
	else if (parser->lexer->token == TK_LLARGE)
	{
		ast->child[1] = parser_compoundstmt(parser);
	}
	return ast;
}


/*
argsdef:
	| type NAME ["=" exp] ("," type NAME ["=" exp])* ["," ...]
*/
struct AST* parser_argsdef(struct Parser* parser)
{
	struct AST* ast = ast_new(NT_ARGDEF, parser);
	struct AST* p = ast;
	p->type = parser_type(parser);
	p->attr.sval = parser->lexer->tokenstr;
	parser_match(parser, TK_NAME);
	if (parser->lexer->token == TK_ASSIGN)
	{
		parser_match(parser, TK_ASSIGN);
		p->child[0] = parser_exp(parser);
	}
	while (parser->lexer->token == TK_COMMA)
	{
		struct AST* t = p;
		parser_match(parser, TK_COMMA);
		if (parser->lexer->token == TK_ELLIPSIS)
		{
			parser_match(parser, TK_ELLIPSIS);
			p->sibling = ast_new(NT_ELLIPSIS, parser);
			break;
		}
		p->sibling = ast_new(NT_ARGDEF, parser);
		p = p->sibling;
		p->type = parser_type(parser);
		p->attr.sval = parser->lexer->tokenstr;
		parser_match(parser, TK_NAME);
		if (parser->lexer->token == TK_ASSIGN)
		{
			parser_match(parser, TK_ASSIGN);
			p->child[0] = parser_exp(parser);
		}
		p->last_sibling = t;
	}
	return ast;
}

/*
returnstmt:
	| "return" [exp]
*/
struct AST* parser_returnstmt(struct Parser* parser)
{
	struct AST* ast = ast_new(NT_RETURNSTMT, parser);
	parser_match(parser, KW_RETURN);
	ast->child[0] = parser_exp(parser);
	return ast;
}

/*
breakstmt:
	| "break"
*/
struct AST* parser_breakstmt(struct Parser* parser)
{
	struct AST* ast = ast_new(NT_BREAKSTMT, parser);
	parser_match(parser, KW_BREAK);
	return ast;
}

/*
continuestmt:
	| "continue"
*/
struct AST* parser_continuestmt(struct Parser* parser)
{
	struct AST* ast = ast_new(NT_CONTINUESTMT, parser);
	parser_match(parser, KW_CONTINUE);
	return ast;
}

/*
externstmt:
	| "extern" "{" statements "}"
	| "extern" statement
*/
struct AST* parser_externstmt(struct Parser* parser)
{
	struct AST* ast = ast_new(NT_EXTERNSTMT, parser);
	parser_match(parser, KW_EXTERN);
	if (parser->lexer->token == TK_LLARGE)
	{
		parser_match(parser, TK_LLARGE);
		ast->child[0] = parser_statements(parser);
		parser_match(parser, TK_RLARGE);
	}
	else
	{
		ast->child[0] = parser_statement(parser);
	}
	return ast;
}

/*
whilestmt:
	| "while" exp statement
*/
struct AST* parser_whilestmt(struct Parser* parser)
{
	struct AST* ast = ast_new(NT_WHILESTMT, parser);
	parser_match(parser, KW_WHILE);
	ast->child[0] = parser_exp(parser);
	ast->child[1] = parser_statement(parser);
	return ast;
}

/*
structdef:
	| "struct" NAME "{" defstmt* "}"
*/
struct AST* parser_structdef(struct Parser* parser)
{
	struct AST* ast = ast_new(NT_STRUCTDEF, parser);
	parser_match(parser, KW_STRUCT);
	ast->attr.sval = parser->lexer->tokenstr;
	parser_match(parser, TK_NAME);
	parser_match(parser, TK_LLARGE);
	if (parser->lexer->token == KW_VAR)
	{
		struct AST* p = parser_defstmt(parser);
		ast->child[0] = p;
		while (parser->lexer->token == KW_VAR)
		{
			p->sibling = parser_defstmt(parser);
			p = p->sibling;
		}
	}
	parser_match(parser, TK_RLARGE);
	return ast;
}