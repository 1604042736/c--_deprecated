#include "type.h"
#include "lexer.h"

struct Type* type_new(enum TypeType typetype)
{
	struct Type* type = (struct Type*)malloc(sizeof(struct Type));
	if (type == NULL)
	{
		printf("AST分配内存失败");
		exit(1);
	}
	type->typetype = typetype;
	for (int i = 0; i < MAX_TYPECHILD_NUM; i++)
	{
		type->child[i] = NULL;
	}
	type->sibling = NULL;
	type->last_sibling = NULL;
	type->attribute = 0;
	return type;
}

int type_getsize(struct Type* type)
{
	switch (type->typetype)
	{
	case T_INT:
		return 4;
	case T_CHAR:
		return 1;
	case T_POINTER:
		return 4;
	case T_ARRAY:
		return type_getsize(type->child[0]) * type->attr.ival;
	case T_STRUCT:
	{
		int size = 0;
		struct Type* p = type->child[0];
		while (p != NULL)
		{
			size += type_getsize(p);
			p = p->sibling;
		}
		return size;
	}
	default:
		return 0;
	}
}

void type_print(struct Type* type)
{
	switch (type->typetype)
	{
	case T_INT:
		printf("int");
		break;
	case T_CHAR:
		printf("char");
		break;
	case T_POINTER:
		type_print(type->child[0]);
		printf("*");
		break;
	case T_FUNC:
	{
		type_print(type->child[0]);
		printf("(");
		struct Type* p = type->child[1];
		if (p != NULL)
		{
			while (p->sibling != NULL)
			{
				type_print(p);
				printf(",");
				p = p->sibling;
			}
			type_print(p);
		}
		if (CHECK_FLAG(type->attribute, TF_INDEFARG))
		{
			if (p != NULL)
			{
				printf(",");
			}
			printf("...");
		}
		printf(")");
		break;
	}
	case T_ARRAY:
		type_print(type->child[0]);
		printf("[%d]", type->attr.ival);
		break;
	case T_VOID:
		printf("void");
		break;
	case T_STRUCT:
	{
		printf("struct %s{", type->attr.sval);
		struct Type* p = type->child[0];
		if (p != NULL)
		{
			while (p->sibling != NULL)
			{
				type_print(p);
				printf(",");
				p = p->sibling;
			}
			type_print(p);
		}
		printf("}");
		break;
	}
	}
}

int type_compare(struct Type* type, struct Type* other)
{
	if (type == NULL && other == NULL)return 1;
	if (type == NULL || other == NULL)return 0;
	if (type->typetype != other->typetype)return 0;
	if (!(type->attr.ival == other->attr.ival || !strcmp(type->attr.sval, other->attr.sval)))return 0;
	for (int i = 0; i < MAX_TYPECHILD_NUM; i++)
	{
		if (type_compare(type->child[i], other->child[i]) == 0)
		{
			return 0;
		}
	}
	return 1;
}