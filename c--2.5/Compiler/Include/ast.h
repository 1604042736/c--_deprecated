#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "type.h"
#include "symtab.h"
#include "parser.h"

#define MAX_ASTCHILD_NUM 4

enum NodeType
{
	NT_PROGRAM,
	NT_COMPOUNDSTMT, NT_IFSTMT,NT_DEFSTMT, NT_RETURNSTMT, NT_BREAKSTMT, NT_CONTINUESTMT, NT_EXTERNSTMT,NT_WHILESTMT,
	NT_BINOP, NT_UNARYOP,
	NT_NAME,NT_INT,NT_DEC,NT_STRING,NT_CHAR,
	NT_VARDEF,NT_FUNCDEF,NT_ARGDEF,NT_CALL,NT_MEMBER,NT_SUBSCRIPT,NT_STRUCTDEF,
	NT_IMPLICITCAST,NT_ELLIPSIS
};

struct AST
{
	enum NodeType nodetype;
	struct AST* child[MAX_ASTCHILD_NUM];
	struct AST* sibling;
	struct AST* last_sibling;
	struct Type* type;
	union
	{
		char* sval;
		int ival;
		double dval;
	}attr;

	char* linecode;
	int line, column;
	int start_column;
	char* filename;
	struct Parser* parser;
};

struct AST* ast_new(enum NodeType,struct Parser*);
struct AST* ast_new2(enum NodeType);
void ast_print(struct AST*, int indent);
void ast_typecheck(struct AST*,struct Symtab*);
void ast_error(struct AST*, char*, ...);