#include "Include/analyzer.h"

struct Analyzer* analyzer_init(struct SyntaxTree* syntaxtree, struct Symbol* symbol)
{
	struct Analyzer* analyzer = (struct Analyzer*)malloc(sizeof(struct Analyzer));
	if (NULL == analyzer)
	{
		printf("analyzer初始化失败");
		exit(-1);
	}
	analyzer->syntaxtree = syntaxtree;
	analyzer->symbol = symbol;
	return analyzer;
}

void analyzer_check(struct Analyzer* analyzer, struct SyntaxTree* t, char* filename)
{
	if (NULL == t)
	{
		return;
	}
	if (t->asttype == AT_SYNTAXTREE)
	{
		/*先检查子节点*/
		for (int i = 0; i < NP_LEN; i++)
		{
			if (t->child[i] != NULL)
			{
				if (t->child[i]->asttype != AT_INT && t->child[i]->asttype != AT_STRING)
				{
					analyzer_check(analyzer, t->child[i], filename);
				}
			}
		}
		switch (t->nodetype)
		{
		case NT_NAME:
		{
			struct SymbolItem* si = symbol_search(analyzer->symbol, t->child[NP_ID]->string);
			if (NULL == si)
			{
				error("未声明的变量'%s'", filename, t->lineno, t->linepos, t->child[NP_ID]->string);
			}
			break;
		}
		case NT_CALL:
		{
			/*以下的分析只针对直接调用的函数*/
			struct SyntaxTree* target = t->child[NP_TARGET];
			if (target!=NULL&&target->nodetype == NT_NAME)
			{
				/*获取调用函数参数*/
				char* name = target->child[NP_ID]->string;
				struct SymbolItem* si = symbol_search(analyzer->symbol, name);
				struct SyntaxTree* args = si->args;
				if (args->list->len != t->child[NP_ARGS]->list->len)
				{
					error("实参与形参数量不符", filename, t->lineno, t->linepos);
				}
				for (int i = 0; i < args->list->len; i++)
				{
					struct SyntaxTree* param = syntaxtreelist_index(args->list, i);	//形参
					struct SyntaxTree* param2 = syntaxtreelist_index(t->child[NP_ARGS]->list, i);	//实参
					si = symbol_search(analyzer->symbol, param->child[NP_ID]->string);
					if (param2->child[NP_ID] != NULL)	//如果参数是单纯的变量
					{
						struct SymbolItem* si2 = symbol_search(analyzer->symbol, param2->child[NP_ID]->string);
						if (si->type == T_STRUCT && si2->type == T_STRUCT)
						{
							if (strcmp(si->struct_type, si2->struct_type))
							{
								warning("实参'%s'类型与形参'%s'不符", filename, t->lineno, t->linepos, si2->struct_type, si->struct_type);
							}
						}
						else if (si->type == T_ARRAY && si2->type == T_ARRAY)
						{
							if (strcmp(si->array_type, si2->array_type))
							{
								warning("实参'%s'类型与形参'%s'不符", filename, t->lineno, t->linepos, si2->array_type, si->array_type);
							}
						}
						else if (si->type != si2->type)
						{
							warning("实参'%d'类型与形参'%d'不符", filename, t->lineno, t->linepos, si2->type, si->type);
						}
					}
				}
			}
		}
		}
	}
	else if (t->asttype == AT_LIST)
	{
		struct _SyntaxTreeList* p = t->list->alltreelist;
		while (p != NULL)
		{
			for (int i = 0; i < MAXSYNTAXTREELISTLEN; i++)
			{
				if (p->syntaxtree[i] != NULL)
				{
					analyzer_check(analyzer, p->syntaxtree[i], filename);
				}
			}
			p = p->next;
		}
	}
}