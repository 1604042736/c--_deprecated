#include "Include/generator.h"

struct Ins* ins_init(char* op, char* arg1, char* arg2)
{
	struct Ins* ins = (struct Ins*)malloc(sizeof(struct Ins));
	if (NULL == ins)
	{
		printf("Ins初始化失败");
		exit(-1);
	}
	ins->op = op;
	ins->arg1 = arg1;
	ins->arg2 = arg2;
	return ins;
}

struct _Code* _code_init()
{
	struct _Code* _code = (struct _Code*)malloc(sizeof(struct _Code));
	if (NULL == _code)
	{
		printf("_Code初始化失败");
		exit(-1);
	}
	_code->nowpos = 0;
	_code->next = NULL;
	for (int i = 0; i < MAXINSLEN; i++)
	{
		_code->ins[i] = NULL;
	}
	return _code;
}

struct Code* code_init()
{
	struct Code* code = (struct Code*)malloc(sizeof(struct Code));
	if (NULL == code)
	{
		printf("Code初始化失败");
		exit(-1);
	}
	code->allcode = _code_init();
	code->nowcode = code->allcode;
	return code;
}

struct Section* section_init(char* name)
{
	struct Section* section = (struct Section*)malloc(sizeof(struct Section));
	if (NULL == section)
	{
		printf("Section初始化失败");
		exit(-1);
	}
	section->name = name;
	section->code = code_init();
	return section;
}

void code_findempty(struct Code* code)
{
	struct _Code* p = code->allcode;
	while (p->next != NULL)
	{
		for (int i = 0; i < MAXINSLEN; i++)
		{
			if (p->ins[i] != NULL)
			{
				p->nowpos = i;
				code->nowcode = p;
				return;
			}
		}
		if (p->next == NULL)
		{
			break;
		}
		p = p->next;
	}
	struct _Code* newcode = _code_init();
	code->nowcode = newcode;
	p->next = newcode;
}

void code_add(struct Code* code, struct Ins* ins)
{
	code->nowcode->ins[code->nowcode->nowpos] = ins;
	code_findempty(code);
}