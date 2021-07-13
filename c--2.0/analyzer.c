#include "Include/analyzer.h"

struct Analyzer* analyzer_init(struct SyntaxTree* syntaxtree, struct Symbol* symbol)
{
	struct Analyzer* analyzer = (struct Analyzer*)malloc(sizeof(struct Analyzer));
	if (NULL == analyzer)
	{
		printf("analyzer��ʼ��ʧ��");
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
		/*�ȼ���ӽڵ�*/
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
				error("δ�����ı���'%s'", filename, t->lineno, t->linepos, t->child[NP_ID]->string);
			}
			break;
		}
		case NT_CALL:
		{
			/*���µķ���ֻ���ֱ�ӵ��õĺ���*/
			struct SyntaxTree* target = t->child[NP_TARGET];
			if (target!=NULL&&target->nodetype == NT_NAME)
			{
				/*��ȡ���ú�������*/
				char* name = target->child[NP_ID]->string;
				struct SymbolItem* si = symbol_search(analyzer->symbol, name);
				struct SyntaxTree* args = si->args;
				if (args->list->len != t->child[NP_ARGS]->list->len)
				{
					error("ʵ�����β���������", filename, t->lineno, t->linepos);
				}
				for (int i = 0; i < args->list->len; i++)
				{
					struct SyntaxTree* param = syntaxtreelist_index(args->list, i);	//�β�
					struct SyntaxTree* param2 = syntaxtreelist_index(t->child[NP_ARGS]->list, i);	//ʵ��
					si = symbol_search(analyzer->symbol, param->child[NP_ID]->string);
					if (param2->child[NP_ID] != NULL)	//��������ǵ����ı���
					{
						struct SymbolItem* si2 = symbol_search(analyzer->symbol, param2->child[NP_ID]->string);
						if (si->type == T_STRUCT && si2->type == T_STRUCT)
						{
							if (strcmp(si->struct_type, si2->struct_type))
							{
								warning("ʵ��'%s'�������β�'%s'����", filename, t->lineno, t->linepos, si2->struct_type, si->struct_type);
							}
						}
						else if (si->type == T_ARRAY && si2->type == T_ARRAY)
						{
							if (strcmp(si->array_type, si2->array_type))
							{
								warning("ʵ��'%s'�������β�'%s'����", filename, t->lineno, t->linepos, si2->array_type, si->array_type);
							}
						}
						else if (si->type != si2->type)
						{
							warning("ʵ��'%d'�������β�'%d'����", filename, t->lineno, t->linepos, si2->type, si->type);
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