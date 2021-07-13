#include "Include/globals.h"

struct SyntaxTree* syntaxtree_init(enum NodeType nodetype, enum ASTType asttype, int lineno, int linepos)
{
	struct SyntaxTree* syntaxtree = (struct SyntaxTree*)malloc(sizeof(struct SyntaxTree));
	if (NULL == syntaxtree)
	{
		printf("初始化语法树失败");
		exit(-1);
	}
	syntaxtree->nodetype = nodetype;
	syntaxtree->asttype = asttype;
	syntaxtree->lineno = lineno;
	syntaxtree->linepos = linepos;
	for (int i = 0; i < NP_LEN; i++)
	{
		syntaxtree->child[i] = NULL;
	}
	switch (syntaxtree->asttype)
	{
	case AT_SYNTAXTREE:
		break;
	case AT_LIST:
		syntaxtree->list = syntaxtreelist_init();
		break;
	case AT_STRING:
		syntaxtree->string = "\0";
		break;
	case AT_INT:
		syntaxtree->value = 0;
		break;
	}
	return syntaxtree;
}

struct _SyntaxTreeList* _syntaxtreelist_init()
{
	struct _SyntaxTreeList* _stl = (struct _SyntaxTreeList*)malloc(sizeof(struct _SyntaxTreeList));
	if (NULL == _stl)
	{
		printf("初始化语法树列表(_SyntaxTreeList)失败");
		exit(-1);
	}
	_stl->nowpos = 0;
	_stl->next = NULL;
	for (int i = 0; i < MAXSYNTAXTREELISTLEN; i++)
	{
		_stl->syntaxtree[i] = NULL;
	}
	return _stl;
}

struct SyntaxTreeList* syntaxtreelist_init()
{
	struct SyntaxTreeList* stl = (struct SyntaxTreeList*)malloc(sizeof(struct SyntaxTreeList));
	if (NULL == stl)
	{
		printf("初始化语法树列表(SyntaxTreeList)失败");
		exit(-1);
	}
	stl->alltreelist = _syntaxtreelist_init();
	stl->nowtreelist = stl->alltreelist;
	stl->len = 0;
	return stl;
}

void syntaxtreelist_add(struct SyntaxTreeList* syntaxtreelist, struct SyntaxTree* syntaxtree)
{
	syntaxtreelist->len++;
	syntaxtreelist->nowtreelist->syntaxtree[syntaxtreelist->nowtreelist->nowpos] = syntaxtree;
	syntaxtreelist_findempty(syntaxtreelist);
}

void syntaxtreelist_findempty(struct SyntaxTreeList* syntaxtreelist)
{
	struct _SyntaxTreeList* p = syntaxtreelist->alltreelist;
	while (p != NULL)
	{
		for (int i = 0; i < MAXSYNTAXTREELISTLEN; i++)
		{
			if (p->syntaxtree[i] == NULL)
			{
				p->nowpos = i;
				syntaxtreelist->nowtreelist = p;
				return;
			}
		}
		if (p->next == NULL)
		{
			break;
		}
		p = p->next;
	}
	struct _SyntaxTreeList* newstl = _syntaxtreelist_init();
	syntaxtreelist->nowtreelist = newstl;
	p->next = newstl;
}

/*获取指定索引的语法树*/
struct SyntaxTree* syntaxtreelist_index(struct SyntaxTreeList* syntaxtreelist,int index)
{
	struct _SyntaxTreeList* p = syntaxtreelist->alltreelist;
	int j = -1;
	while (p != NULL)
	{
		for (int i = 0; i < MAXSYNTAXTREELISTLEN; i++)
		{
			if (p->syntaxtree[i] != NULL)
			{
				j++;
				if (j == index)
				{
					return p->syntaxtree[i];
				}
			}
		}
		p = p->next;
	}
	return NULL;
}

void syntaxtree_print(struct SyntaxTree* syntaxtree, int space)
{
	if (syntaxtree == NULL)
	{
		return;
	}
	if (syntaxtree->asttype == AT_SYNTAXTREE)
	{
		for (int i = 0; i < space; i++)
		{
			printf("  ");
		}
		switch (syntaxtree->nodetype)
		{
		case NT_PROGRAM:
			printf("Program\n");
			break;
		case NT_IF:
			printf("If\n");
			break;
		case NT_WHILE:
			printf("While\n");
			break;
		case NT_NAME:
			printf("Name\n");
			break;
		case NT_NUM:
			printf("Num\n");
			break;
		case NT_ADD:
			printf("Add\n");
			break;
		case NT_SUB:
			printf("Sub\n");
			break;
		case NT_MUL:
			printf("Mul\n");
			break;
		case NT_DIV:
			printf("Div\n");
			break;
		case NT_MOD:
			printf("Mod\n");
			break;
		case NT_ASSIGN:
			printf("Assign\n");
			break;
		case NT_GT:
			printf("Gt\n");
			break;
		case NT_GEQ:
			printf("Geq\n");
			break;
		case NT_LT:
			printf("Lt\n");
			break;
		case NT_LEQ:
			printf("Leq\n");
			break;
		case NT_EQ:
			printf("Eq\n");
			break;
		case NT_NEQ:
			printf("Neq\n");
			break;
		case NT_AND:
			printf("And\n");
			break;
		case NT_OR:
			printf("Or\n");
			break;
		case NT_LSHIFT:
			printf("LShift\n");
			break;
		case NT_RSHIFT:
			printf("RShift\n");
			break;
		case NT_CALL:
			printf("Call\n");
			break;
		case NT_STRING:
			printf("String\n");
			break;
		case NT_CHAR:
			printf("Char\n");
			break;
		case NT_INDEX:
			printf("Index\n");
			break;
		case NT_BREAK:
			printf("Break\n");
			break;
		case NT_CONTINUE:
			printf("Continue\n");
			break;
		case NT_RETURN:
			printf("Return\n");
			break;
		case NT_ATTR:
			printf("Attr\n");
			break;
		case NT_DEFINE:
			printf("Define\n");
			break;
		case NT_FUNCTION:
			printf("Function\n");
			break;
		}
		for (int i = 0; i < NP_LEN; i++)
		{
			if (syntaxtree->child[i] != NULL)
			{
				for (int j = 0; j < space + 1; j++)
				{
					printf("  ");
				}
				switch (i)
				{
				case NP_BODY:
					printf("body");
					break;
				case NP_LEFT:
					printf("left");
					break;
				case NP_RIGHT:
					printf("right");
					break;
				case NP_TARGET:
					printf("target");
					break;
				case NP_VALUE:
					printf("value");
					break;
				case NP_ID:
					printf("id");
					break;
				case NP_EXP:
					printf("exp");
					break;
				case NP_ELSES:
					printf("elses");
					break;
				case NP_ARGS:
					printf("args");
					break;
				case NP_ATTR:
					printf("attr");
					break;
				case NP_DEFAULT:
					printf("default");
					break;
				}
				if (syntaxtree->child[i]->asttype != AT_INT && syntaxtree->child[i]->asttype != AT_STRING)
				{
					printf("\n");
					syntaxtree_print(syntaxtree->child[i], space + 2);
				}
				else
				{
					switch (syntaxtree->child[i]->asttype)
					{
					case AT_INT:
						printf(": %d\n", syntaxtree->child[i]->value);
						break;
					case AT_STRING:
						printf(": %s\n", syntaxtree->child[i]->string);
						break;
					}
				}
			}
		}
	}
	else if (syntaxtree->asttype == AT_LIST)
	{
		struct _SyntaxTreeList* p = syntaxtree->list->alltreelist;
		while (p != NULL)
		{
			for (int i = 0; i < MAXSYNTAXTREELISTLEN; i++)
			{
				if (p->syntaxtree[i] != NULL)
				{
					syntaxtree_print(p->syntaxtree[i], space);
				}
			}
			p = p->next;
		}
	}
}