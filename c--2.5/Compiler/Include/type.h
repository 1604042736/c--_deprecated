#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "symtab.h"

#define MAX_TYPECHILD_NUM 2

#define TF_INDEFARG 1

enum TypeType
{
	T_INT,T_CHAR,T_POINTER,T_FUNC,T_ARRAY,T_VOID,T_STRUCT
};

struct Type
{
	enum TypeType typetype;
	struct Type* child[MAX_TYPECHILD_NUM];
	struct Type* sibling;
	struct Type* last_sibling;
	int attribute;
	union
	{
		int ival;
		char* sval;
	}attr;
};

struct Type* type_new(enum TypeType);
int type_getsize(struct Type*);
void type_print(struct Type*);
int type_compare(struct Type*, struct Type*);