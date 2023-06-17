#include "generator.h"

struct Generator* generator_new(char* filename)
{
	struct Generator* generator = (struct Generator*)malloc(sizeof(struct Generator));
	if (generator == NULL)
	{
		printf("创建Generator失败");
		exit(1);
	}
	generator->file = fopen(filename, "w");
	generator->label_count = 0;
	generator->const_count = 0;

	generator->scope_len = 1;
	generator->scope_size = SCOPE_EXPAND;
	generator->scope_count = 0;
	generator->scope = (int*)malloc(generator->scope_size * sizeof(int));
	CHECK_MALLOC(generator->scope);
	generator->scope[generator->scope_len - 1] = generator->scope_count;

	generator->loop_start_label = generator->loop_end_label = -1;
	return generator;
}

void generator_generate(struct Generator* generator, struct Symtab* symtab, struct AST* ast)
{
	BOTHPRINT(generator->file, "global _main\n");
	generator_generate_data(generator, symtab);
	generator_generate_text(generator, symtab, ast);
}

void generator_generate_data(struct Generator* generator, struct Symtab* symtab)
{
	generator_write0(generator, "section .data:");
	for (int i = 0; i < symtab->symbol_size; i++)
	{
		if (symtab->symbols[i] == NULL)
		{
			continue;
		}
		struct Symbol* p = symtab->symbols[i];
		while (p != NULL)
		{
			char* varname = (char*)malloc(sizeof(char) * 20);
			sprintf(varname, "const%d\0", generator->const_count++);
			if (CHECK_FLAG(p->flag, SF_CONST))
			{
				//必须是字符串
				if (p->type->typetype != T_ARRAY && p->type->child[0]->typetype != T_CHAR)
				{
					continue;
				}
				BOTHPRINT(generator->file, "%s db ", varname);
				for (int i = 0; i < strlen(p->name); i++)
				{
					char c = p->name[i];
					switch (c)
					{
					case '\n':
						BOTHPRINT(generator->file, "0ah");
						break;
					case '\0':
						BOTHPRINT(generator->file, "0");
						break;
					default:
						BOTHPRINT(generator->file, "'%c'", c);
						break;
					}
					BOTHPRINT(generator->file, ",");

				}
				BOTHPRINT(generator->file, "0");
				BOTHPRINT(generator->file, "\n");
				p->cons.dataname = varname;
			}
			else if (CHECK_FLAG(p->flag, SF_EXTERN))
			{
				BOTHPRINT(generator->file, "extern %s\n", p->anname);
			}
			else if (p->scope_size == 1 && p->type->typetype != T_FUNC)	//全局变量
			{
				BOTHPRINT(generator->file, "%s dw 0\n", p->name);
			}
			p = p->nextsame;
		}
	}
}

void generator_generate_text(struct Generator* generator, struct Symtab* symtab, struct AST* ast)
{
	generator_write0(generator, "section .text:");
	_generator_generate_text(generator, symtab, ast);
}

void _generator_generate_text(struct Generator* generator, struct Symtab* symtab, struct AST* ast)
{
	while (ast != NULL)
	{
		generator_single(generator, symtab, ast);
		ast = ast->sibling;
	}
}

void generator_single(struct Generator* generator, struct Symtab* symtab, struct AST* ast)
{
	if (ast == NULL)return;
	switch (ast->nodetype)
	{
	case NT_STRUCTDEF:
		generator_enterscope(generator);
		generator_leavescope(generator);
		break;
	case NT_FUNCDEF:
	{
		if (ast->child[1] == NULL)	//语义分析时这种情况会更新作用域
		{
			generator_enterscope(generator);
			generator_leavescope(generator);
			break;
		}
		generator_enterscope(generator);
		struct Symbol* sym = symtab_lookup(symtab, ast->attr.sval, generator->scope, generator->scope_len);
		generator_writelabel2(generator, sym->anname);
		generator_write1(generator, "push", "ebp");
		generator_write2(generator, "mov", "ebp", "esp");
		char buff[10];
		generator_write2(generator, "sub", "esp", _itoa(sym->func.scopeesp, buff, 10));

		_generator_generate_text(generator, symtab, ast->child[1]);

		generator_write2(generator, "mov", "esp", "ebp");
		generator_write1(generator, "pop", "ebp");
		generator_write0(generator, "ret");
		generator_leavescope(generator);
		break;
	}
	case NT_CALL:
	{
		struct AST* p = ast->child[1];
		while (p->sibling != NULL)p = p->sibling;
		int size = 0;
		while (p != NULL)
		{
			size += type_getsize(p->type);
			generator_single(generator, symtab, p);
			generator_write1(generator, "push", "eax");
			p = p->last_sibling;
		}

		_generator_generate_text(generator, symtab, ast->child[0]);
		generator_write1(generator, "call", "eax");

		char buff[10];
		generator_write2(generator, "add", "esp", _itoa(size, buff, 10));
		break;
	}
	case NT_CHAR:
	{
		int ascii = ast->attr.sval[0];
		char buff[1024];
		_itoa(ascii, buff, 16);
		sprintf(buff, "%sh\0", _itoa(ascii, buff, 16));
		generator_write2(generator, "mov", "eax", buff);
		break;
	}
	case NT_STRING:
	{
		struct Symbol* sym = symtab_lookup(symtab, ast->attr.sval, generator->scope, generator->scope_len);
		generator_write2(generator, "mov", "eax", sym->cons.dataname);
		break;
	}
	case NT_NAME:
	{
		struct Symbol* sym = symtab_lookup(symtab, ast->attr.sval, generator->scope, generator->scope_len);
		if (sym == NULL)
		{
			ast_error(ast, "找不到'%s'", ast->attr.sval);
		}
		if (sym->type->typetype == T_FUNC)
		{
			generator_write2(generator, "mov", "eax", sym->anname);
		}
		else if (sym->scope_size == 1)
		{
			char buff[100];
			sprintf(buff, "%s", sym->name);
			generator_write2(generator, "mov", "eax", sym->name);
			generator_write2(generator, "lea", "ecx", buff);
		}
		else if (sym->type->typetype == T_ARRAY)
		{
			char* buff = (char*)malloc(sizeof(char) * 100);
			CHECK_MALLOC(buff);
			if (sym->var.offset > 0) sprintf(buff, "[ebp-%d]", sym->var.offset);
			else sprintf(buff, "[ebp+%d]", -sym->var.offset);
			generator_write2(generator, "lea", "eax", buff);
			generator_write2(generator, "lea", "ecx", buff);
		}
		else
		{
			char* buff = (char*)malloc(sizeof(char) * 100);
			CHECK_MALLOC(buff);
			if (sym->var.offset > 0) sprintf(buff, "[ebp-%d]", sym->var.offset);
			else sprintf(buff, "[ebp+%d]", -sym->var.offset);
			generator_write2(generator, "mov", "eax", buff);
			generator_write2(generator, "lea", "ecx", buff);
		}
		break;
	}
	case NT_RETURNSTMT:
		_generator_generate_text(generator, symtab, ast->child[0]);
		generator_write2(generator, "mov", "esp", "ebp");
		generator_write1(generator, "pop", "ebp");
		generator_write0(generator, "ret");
		break;
	case NT_INT:
	{
		char v[100];
		generator_write2(generator, "mov", "eax", _itoa(ast->attr.ival, v, 10));
		break;
	}
	case NT_IFSTMT:
	{
		int elselabel = generator_getlabel(generator);
		int endlabel = generator_getlabel(generator);
		_generator_generate_text(generator, symtab, ast->child[0]);
		generator_write2(generator, "cmp", "eax", "0");
		generator_write1(generator, "je", generator_getlabelstr(generator, elselabel));

		_generator_generate_text(generator, symtab, ast->child[1]);
		generator_write1(generator, "jmp", generator_getlabelstr(generator, endlabel));

		generator_writelabel(generator, elselabel);
		_generator_generate_text(generator, symtab, ast->child[2]);

		generator_writelabel(generator, endlabel);
		break;
	}
	case NT_BINOP:
	{
#define ARITHOP(a) _generator_generate_text(generator, symtab, ast->child[0]);\
		generator_write1(generator, "push", "eax");\
		_generator_generate_text(generator, symtab, ast->child[1]);\
		generator_write1(generator, "pop", "ebx");\
		generator_write2(generator, a, "ebx", "eax");\
		generator_write2(generator, "mov", "eax", "ebx");

#define COMPAREOP(a) int tlabel = generator_getlabel(generator);\
		int endlabel = generator_getlabel(generator);\
		_generator_generate_text(generator, symtab, ast->child[0]);\
		generator_write1(generator, "push", "eax");\
		_generator_generate_text(generator, symtab, ast->child[1]);\
		generator_write1(generator, "pop", "ebx");\
		generator_write2(generator, "cmp", "ebx", "eax");\
		generator_write1(generator, a, generator_getlabelstr(generator, tlabel));\
		generator_write2(generator, "mov", "eax", "0");\
		generator_write1(generator, "jmp", generator_getlabelstr(generator, endlabel));\
		generator_writelabel(generator, tlabel);\
		generator_write2(generator, "mov", "eax", "1");\
		generator_writelabel(generator, endlabel);

		if (!strcmp(ast->attr.sval, "or"))
		{
			int tlabel = generator_getlabel(generator);
			int endlabel = generator_getlabel(generator);

			_generator_generate_text(generator, symtab, ast->child[0]);
			generator_write2(generator, "cmp", "eax", "1");
			generator_write1(generator, "je", generator_getlabelstr(generator, tlabel));
			_generator_generate_text(generator, symtab, ast->child[1]);
			generator_write2(generator, "cmp", "eax", "1");
			generator_write1(generator, "je", generator_getlabelstr(generator, tlabel));

			generator_write2(generator, "mov", "eax", "0");
			generator_write1(generator, "jmp", generator_getlabelstr(generator, endlabel));
			generator_writelabel(generator, tlabel);
			generator_write2(generator, "mov", "eax", "1");
			generator_writelabel(generator, endlabel);
		}
		else if (!strcmp(ast->attr.sval, "and"))
		{
			int flabel = generator_getlabel(generator);
			int endlabel = generator_getlabel(generator);

			_generator_generate_text(generator, symtab, ast->child[0]);
			generator_write2(generator, "cmp", "eax", "0");
			generator_write1(generator, "je", generator_getlabelstr(generator, flabel));
			_generator_generate_text(generator, symtab, ast->child[1]);
			generator_write2(generator, "cmp", "eax", "0");
			generator_write1(generator, "je", generator_getlabelstr(generator, flabel));

			generator_write2(generator, "mov", "eax", "1");
			generator_write1(generator, "jmp", generator_getlabelstr(generator, endlabel));
			generator_writelabel(generator, flabel);
			generator_write2(generator, "mov", "eax", "0");
			generator_writelabel(generator, endlabel);
		}
		else if (!strcmp(ast->attr.sval, "&"))
		{
			ARITHOP("and");
		}
		else if (!strcmp(ast->attr.sval, "|"))
		{
			ARITHOP("or");
		}
		else if (!strcmp(ast->attr.sval, "^"))
		{
			ARITHOP("xor");
		}
		else if (!strcmp(ast->attr.sval, ">>"))
		{
			ARITHOP("shr");
		}
		else if (!strcmp(ast->attr.sval, "<<"))
		{
			ARITHOP("shl");
		}
		else if (!strcmp(ast->attr.sval, "+"))
		{
			ARITHOP("add");
		}
		else if (!strcmp(ast->attr.sval, "-"))
		{
			ARITHOP("sub");
		}
		else if (!strcmp(ast->attr.sval, "*"))
		{
			ARITHOP("imul");
		}
		else if (!strcmp(ast->attr.sval, "/"))
		{
			ARITHOP("idiv");
		}
		else if (!strcmp(ast->attr.sval, "=="))
		{
			COMPAREOP("je");
		}
		else if (!strcmp(ast->attr.sval, "<="))
		{
			COMPAREOP("jle");
		}
		else if (!strcmp(ast->attr.sval, "<"))
		{
			COMPAREOP("jl");
		}
		else if (!strcmp(ast->attr.sval, ">"))
		{
			COMPAREOP("jg");
		}
		else if (!strcmp(ast->attr.sval, ">="))
		{
			COMPAREOP("jge");
		}
		else if (!strcmp(ast->attr.sval, "!="))
		{
			COMPAREOP("jne");
		}
		else if (!strcmp(ast->attr.sval, "="))
		{
			_generator_generate_text(generator, symtab, ast->child[1]);
			generator_write1(generator, "push", "eax");
			_generator_generate_text(generator, symtab, ast->child[0]);
			generator_write1(generator, "pop", "eax");
			if (type_getsize(ast->child[0]->type) == 1)
			{
				generator_write2(generator, "mov", "[ecx]", "al");
			}
			else
			{
				generator_write2(generator, "mov", "[ecx]", "eax");
			}
		}
		break;
	}
	case NT_COMPOUNDSTMT:
		generator_enterscope(generator);
		_generator_generate_text(generator, symtab, ast->child[0]);
		generator_leavescope(generator);
		break;
	case NT_WHILESTMT:
	{
		int _lsl = generator->loop_start_label;
		int _lel = generator->loop_end_label;

		int startlabel = generator_getlabel(generator);
		int endlabel = generator_getlabel(generator);
		generator->loop_start_label = startlabel;
		generator->loop_end_label = endlabel;

		generator_writelabel(generator, startlabel);
		_generator_generate_text(generator, symtab, ast->child[0]);
		generator_write2(generator, "cmp", "eax", "0");
		generator_write1(generator, "je", generator_getlabelstr(generator, endlabel));

		_generator_generate_text(generator, symtab, ast->child[1]);
		generator_write1(generator, "jmp", generator_getlabelstr(generator, startlabel));
		generator_writelabel(generator, endlabel);

		generator->loop_start_label = _lsl;
		generator->loop_end_label = _lel;
		break;
	}
	case NT_BREAKSTMT:
	{
		if (generator->loop_end_label != -1)
		{
			generator_write1(generator, "jmp", generator_getlabelstr(generator, generator->loop_end_label));
		}
		else
		{
			ast_error(ast, "Cannot use 'break' out of loop");
		}
		break;
	}
	case NT_CONTINUESTMT:
	{
		if (generator->loop_start_label != -1)
		{
			generator_write1(generator, "jmp", generator_getlabelstr(generator, generator->loop_start_label));
		}
		else
		{
			ast_error(ast, "Cannot use 'continue' out of loop");
		}
		break;
	}
	case NT_SUBSCRIPT:
	{
		_generator_generate_text(generator, symtab, ast->child[1]);
		generator_write1(generator, "push", "eax");
		_generator_generate_text(generator, symtab, ast->child[0]);
		generator_write1(generator, "pop", "ebx");

		char buff[100];
		generator_write2(generator, "imul", "ebx", _itoa(type_getsize(ast->type), buff, 10));

		sprintf(buff, "[ecx+ebx]");
		if (ast->type->typetype == T_ARRAY)
		{
			generator_write2(generator, "lea", "eax", buff);
			generator_write2(generator, "lea", "ecx", buff);
		}
		else
		{
			generator_write2(generator, "mov", "eax", buff);
			generator_write2(generator, "lea", "ecx", buff);
			if (type_getsize(ast->type) == 1)
			{
				generator_write2(generator, "and", "eax", "0fffh");
				generator_write2(generator, "mov", "ah", "0");
			}
		}
		break;
	}
	case NT_UNARYOP:
		if (!strcmp(ast->attr.sval, "&"))
		{
			_generator_generate_text(generator, symtab, ast->child[0]);
			generator_write2(generator, "mov", "eax", "ecx");
		}
		else if (!strcmp(ast->attr.sval, "*"))
		{
			_generator_generate_text(generator, symtab, ast->child[0]);
			generator_write2(generator, "mov", "ecx", "eax");
			generator_write2(generator, "mov", "eax", "[ecx]");
		}
		break;
	case NT_MEMBER:
	{
		_generator_generate_text(generator, symtab, ast->child[0]);
		struct Symbol* sym = symtab_lookup(symtab, ast->child[0]->type->attr.sval, generator->scope, generator->scope_len);
		for (int i = 0; i < sym->struc.member_num; i++)
		{
			if (!strcmp(sym->struc.members[i]->name, ast->attr.sval))
			{
				int offset = sym->struc.members[i]->var.offset;
				char* buff = (char*)malloc(sizeof(char) * 100);
				CHECK_MALLOC(buff);
				sprintf(buff, "[ecx+%d]", offset);
				generator_write2(generator, "mov", "eax", buff);
				generator_write2(generator, "lea", "ecx", buff);
				break;
			}
		}
		break;
	}
	default:
		for (int i = 0; i < MAX_ASTCHILD_NUM; i++)
		{
			if (ast->child[i] != NULL)
			{
				_generator_generate_text(generator, symtab, ast->child[i]);
			}
		}
	}
}

int generator_getlabel(struct Generator* generator)
{
	return generator->label_count++;
}

void generator_writelabel(struct Generator* generator, int label)
{
	BOTHPRINT(generator->file, "@%d:\n", label);
}

void generator_writelabel2(struct Generator* generator, char* label)
{
	BOTHPRINT(generator->file, "%s:\n", label);
}

void generator_write0(struct Generator* generator, char* op)
{
	BOTHPRINT(generator->file, "%s\n", op);
}

void generator_write1(struct Generator* generator, char* op, char* arg1)
{
	BOTHPRINT(generator->file, "%s %s\n", op, arg1);
}
void generator_write2(struct Generator* generator, char* op, char* arg1, char* arg2)
{
	BOTHPRINT(generator->file, "%s %s,%s\n", op, arg1, arg2);
}

char* generator_getlabelstr(struct Generator* generator, int label)
{
	char buff[10];
	char* str = _itoa(label, buff, 10);

	char* a = (char*)malloc(sizeof(char) * (strlen(str) + 1));
	CHECK_MALLOC(a);
	a[0] = '@';
	a[1] = '\0';
	return strcat(a, str);
}

void generator_enterscope(struct Generator* generator)
{
	generator->scope_count++;
	if (++generator->scope_len >= generator->scope_size)
	{
		generator->scope_size += SCOPE_EXPAND;
		int* old_scope = generator->scope;
		generator->scope = (int*)malloc(generator->scope_size * sizeof(int));
		CHECK_MALLOC(generator->scope);
		for (int i = 0; i < generator->scope_size - SCOPE_EXPAND; i++)
		{
			generator->scope[i] = old_scope[i];
		}
	}
	generator->scope[generator->scope_len - 1] = generator->scope_count;
}

void generator_leavescope(struct Generator* generator)
{
	generator->scope_len--;
}