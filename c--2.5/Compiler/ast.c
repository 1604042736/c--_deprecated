#include "ast.h"

struct AST* ast_new(enum NodeType nodetype,struct Parser* parser)
{
	struct AST* ast = (struct AST*)malloc(sizeof(struct AST));
	if (ast == NULL)
	{
		printf("AST分配内存失败");
		exit(1);
	}
	ast->nodetype = nodetype;
	for (int i = 0; i < MAX_ASTCHILD_NUM; i++)
	{
		ast->child[i] = NULL;
	}
	ast->sibling = NULL;
	ast->last_sibling = NULL;
	ast->type = NULL;

	ast->filename = parser->lexer->filename;
	ast->linecode = parser->lexer->linecode;
	ast->line = parser->lexer->line;
	ast->column = parser->lexer->column;
	ast->start_column = parser->lexer->start_column;
	ast->parser = parser;
	return ast;
}

struct AST* ast_new2(enum NodeType nodetype)
{
	struct AST* ast = (struct AST*)malloc(sizeof(struct AST));
	if (ast == NULL)
	{
		printf("AST分配内存失败");
		exit(1);
	}
	ast->nodetype = nodetype;
	for (int i = 0; i < MAX_ASTCHILD_NUM; i++)
	{
		ast->child[i] = NULL;
	}
	ast->sibling = NULL;
	ast->last_sibling = NULL;
	ast->type = NULL;

	ast->filename = "";
	ast->linecode = "";
	ast->line = ast->column = ast->start_column = 0;
	ast->parser = NULL;
	return ast;
}

void ast_print(struct AST* ast, int indent)
{
#define PRINTSPACE(a) for(int i=0;i<a;i++)printf("  ");

	while (ast != NULL)
	{
		PRINTSPACE(indent);
		switch (ast->nodetype)
		{
		case NT_PROGRAM:
			printf("Program");
			break;
		case NT_COMPOUNDSTMT:
			printf("CompoundStmt");
			break;
		case NT_IFSTMT:
			printf("IfStmt");
			break;
		case NT_BINOP:
			printf("BinOp: %s",ast->attr.sval);
			break;
		case NT_NAME:
			printf("Name: %s", ast->attr.sval);
			break;
		case NT_INT:
			printf("Int: %d", ast->attr.ival);
			break;
		case NT_DEC:
			printf("Dec: %f", ast->attr.dval);
			break;
		case NT_STRING:
			printf("String: \"%s\"", ast->attr.sval);
			break;
		case NT_CHAR:
			printf("Char: '%s'", ast->attr.sval);
			break;
		case NT_DEFSTMT:
			printf("DefStmt");
			break;
		case NT_VARDEF:
			printf("VarDef: %s",ast->attr.sval);
			break;
		case NT_FUNCDEF:
			printf("FuncDef: %s",ast->attr.sval);
			break;
		case NT_ARGDEF:
			printf("ArgDef: %s",ast->attr.sval);
			break;
		case NT_RETURNSTMT:
			printf("ReturnStmt");
			break;
		case NT_BREAKSTMT:
			printf("BreakStmt");
			break;
		case NT_CONTINUESTMT:
			printf("ContinueStmt");
			break;
		case NT_CALL:
			printf("Call");
			break;
		case NT_MEMBER:
			printf("Member: %s",ast->attr.sval);
			break;
		case NT_SUBSCRIPT:
			printf("Subscript");
			break;
		case NT_EXTERNSTMT:
			printf("Extern");
			break;
		case NT_WHILESTMT:
			printf("While");
			break;
		case NT_UNARYOP:
			printf("UnaryOp: %s", ast->attr.sval);
			break;
		case NT_STRUCTDEF:
			printf("StructDef: %s", ast->attr.sval);
			break;
		case NT_IMPLICITCAST:
			printf("ImplicitCast");
			break;
		case NT_ELLIPSIS:
			printf("Ellpsis");
			break;
		default:
			printf("%d", ast->nodetype);
		}
		if (ast->type != NULL)
		{
			printf(" \033[1;32m'");
			type_print(ast->type);
			printf("'\033[0m");
		}
		printf(" \033[1;33m<%s,%d,%d,%d>\033[0m\n",ast->filename,ast->line,ast->start_column,ast->column);
		for (int i = 0; i < MAX_ASTCHILD_NUM; i++)
		{
			if (ast->child[i] != NULL)
			{
				ast_print(ast->child[i], indent + 1);
			}
		}
		ast = ast->sibling;
	}

#undef PRINTSPACE
}

void ast_error(struct AST* ast, char* format, ...)
{
	printf("Error:%s:%d:%d:", ast->filename, ast->line, ast->column);
	va_list arg;
	va_start(arg, format);
	vfprintf(stdout, format, arg);
	va_end(arg);
	printf(":\n");
	if (ast->linecode[strlen(ast->linecode) - 1] == '\n')
	{
		printf("%s", ast->linecode);
	}
	else
	{
		printf("%s\n", ast->linecode);
	}
	int i = 0;
	for (; i < ast->start_column - 1; i++)
	{
		if (isspace(ast->linecode[i]))
		{
			printf("%c", ast->linecode[i]);
		}
		else
		{
			printf(" ");
		}
	}
	for (; i < ast->column; i++)
	{
		printf("^");
	}
	printf("\n");
	exit(1);
}