#include "Include/symbol.h"

struct SymbolItem* symbolitem_init(char* name, enum Type type)
{
	struct SymbolItem* symbolitem = (struct SymbolItem*)malloc(sizeof(struct SymbolItem));
	if (NULL == symbolitem)
	{
		printf("symbolitem��ʼ��ʧ��");
		exit(-1);
	}
	symbolitem->name = name;
	symbolitem->type = type;
	symbolitem->array_type = "\t";
	symbolitem->return_type = "\t";
	symbolitem->struct_type = "\t";
	symbolitem->args = NULL;
	return symbolitem;
}

struct _Symbol* _symbol_init()
{
	struct _Symbol* _symbol = (struct _Symbol*)malloc(sizeof(struct _Symbol));
	if (NULL == _symbol)
	{
		printf("_symbol��ʼ��ʧ��");
		exit(-1);
	}
	_symbol->nowpos = 0;
	_symbol->next = NULL;
	for (int i = 0; i < MAXSYMBOLLEN; i++)
	{
		_symbol->symbolitem[i] = NULL;
	}
	return _symbol;
}

struct Symbol* symbol_init()
{
	struct Symbol* symbol = (struct Symbol*)malloc(sizeof(struct  Symbol));
	if (NULL == symbol)
	{
		printf("symbol��ʼ��ʧ��");
		exit(-1);
	}
	symbol->allsymbol = _symbol_init();
	symbol->nowsymbol = symbol->allsymbol;
	return symbol;
}

struct SymbolItem* symbol_search(struct Symbol* symbol, char* name)
{
	struct _Symbol* p = symbol->allsymbol;
	while (p != NULL)
	{
		for (int i = 0; i < MAXSYMBOLLEN; i++)
		{
			if (p->symbolitem[i]!=NULL&&!strcmp(p->symbolitem[i]->name, name))
			{
				return p->symbolitem[i];
			}
		}
		p = p->next;
	}
	return NULL;
}

void symbol_findempty(struct Symbol* symbol)
{
	struct _Symbol* p = symbol->allsymbol;
	while (p->next != NULL)
	{
		for (int i = 0; i < MAXSYMBOLLEN; i++)
		{
			if (p->symbolitem[i] == NULL)
			{
				p->nowpos = i;
				symbol->nowsymbol = p;
				return;
			}
		}
		if (p->next == NULL)
		{
			break;
		}
		p = p->next;
	}
	struct _Symbol* newsymbol = _symbol_init();
	symbol->nowsymbol = newsymbol;
	p->next = newsymbol;
}

void symbol_add(struct Symbol* symbol, struct SymbolItem* symbolitem)
{
	symbol->nowsymbol->symbolitem[symbol->nowsymbol->nowpos] = symbolitem;
	symbol_findempty(symbol);
}

void symbol_print(struct Symbol* symbol)
{
	printf("name\ttype\treturn_type\tarray_type\tstruct_type\n");
	struct _Symbol* p = symbol->allsymbol;
	while (p != NULL)
	{
		for (int i = 0; i < MAXSYMBOLLEN; i++)
		{
			if (p->symbolitem[i] != NULL)
			{
				printf("%s\t", p->symbolitem[i]->name);
				switch (p->symbolitem[i]->type)
				{
				case T_INT:
					printf("int\t");
					break;
				case T_CHAR:
					printf("char\t");
					break;
				case T_ARRAY:
					printf("array\t");
					break;
				case T_STRUCT:
					printf("struct\t");
					break;
				case T_FUNCTION:
					printf("function\t");
					break;
				case T_VOID:
					printf("void\t");
					break;
				case T_STRING:
					printf("string\t");
					break;
				}
				printf("%s\t%s\t%s\n", p->symbolitem[i]->return_type, p->symbolitem[i]->array_type, p->symbolitem[i]->struct_type);
			}
		}
		p = p->next;
	}
}

/*��ͨ������ӽ����ű�*/
struct SymbolItem* symbol_addval(struct Symbol* symbol, char* name, enum Type type)
{
	struct SymbolItem* symbolitem = symbol_search(symbol,name);
	/*û���ҵ����������*/
	if (symbolitem == NULL)
	{
		symbolitem = symbolitem_init(name, type);
		symbol_add(symbol, symbolitem);
	}
	return symbolitem;
}

/*��Ӻ��������ű�*/
void symbol_addfunc(struct Symbol* symbol, char* name, char* return_type,struct SyntaxTree* args)
{
	struct SymbolItem* symbolitem = symbol_addval(symbol, name, T_FUNCTION);
	symbolitem->type = T_FUNCTION;
	symbolitem->return_type = return_type;
	symbolitem->array_type = "\t";
	symbolitem->struct_type = "\t";
	symbolitem->args = args;
}

/*�����������ű�*/
void symbol_addarray(struct Symbol* symbol, char* name, char* array_type)
{
	struct SymbolItem* symbolitem = symbol_addval(symbol, name, T_ARRAY);
	symbolitem->type = T_ARRAY;
	symbolitem->array_type = array_type;
	symbolitem->return_type = "\t";
	symbolitem->struct_type = "\t";
}

/*��ӽṹ������ű�*/
void symbol_addstruct(struct Symbol* symbol, char* name, char* struct_type)
{
	struct SymbolItem* symbolitem = symbol_addval(symbol, name, T_STRUCT);
	symbolitem->type = T_STRUCT;
	symbolitem->struct_type= struct_type;
	symbolitem->array_type = "\t";
	symbolitem->return_type = "\t";
}