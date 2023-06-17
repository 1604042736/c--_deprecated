#include "symtab.h"
#include "lexer.h"

struct Symtab* symtab_new()
{
	struct Symtab* symtab = (struct Symtab*)malloc(sizeof(struct Symtab));
	if (symtab == NULL)
	{
		printf("符号表分配内存失败");
		exit(1);
	}
	symtab->scope_len = 1;
	symtab->scope_size = SCOPE_EXPAND;
	symtab->scope_count = 0;
	symtab->scope = (int*)malloc(symtab->scope_size * sizeof(int));
	CHECK_MALLOC(symtab->scope);
	symtab->scope[symtab->scope_len - 1] = symtab->scope_count;

	symtab->symbol_size = symtab->free_symbol_num = HASHTAB_EXPAND;
	symtab->symbols = (struct Symbol**)malloc(symtab->symbol_size * sizeof(struct Symbol*));
	CHECK_MALLOC(symtab->symbols);
	for (int i = 0; i < symtab->symbol_size; i++)
	{
		symtab->symbols[i] = NULL;
	}
	symtab->curfunc = NULL;
	symtab->globeflag = 0;
	return symtab;
}

void symtab_addsymbol(struct Symtab* symtab, struct Symbol* symbol)
{
	int hash = hash_string(symbol->name) % symtab->symbol_size;
	if (symtab->symbols[hash] == NULL)
	{
		symtab->symbols[hash] = symbol;
		symtab->free_symbol_num--;
	}
	else if (!(CHECK_FLAG(symbol->flag, SF_CONST) && !strcmp(symbol->name, symtab->symbols[hash]->name)))
	{//变量可有多个但常量只有一个
		struct Symbol* p = symtab->symbols[hash];
		while (p->nextsame != NULL)p = p->nextsame;
		p->nextsame = symbol;
	}

	if (symtab->free_symbol_num <= 0)	//扩容
	{
		symtab->symbol_size += HASHTAB_EXPAND;
		symtab->free_symbol_num += HASHTAB_EXPAND;
		struct Symbol** old_symbols = symtab->symbols;
		symtab->symbols = (struct Symbol**)malloc(symtab->symbol_size * sizeof(struct Symbol*));
		CHECK_MALLOC(symtab->symbols);
		for (int i = 0; i < symtab->symbol_size; i++)
		{
			symtab->symbols[i] = NULL;
		}
		for (int i = 0; i < symtab->symbol_size - HASHTAB_EXPAND; i++)
		{
			struct Symbol* p = old_symbols[i];
			while (p != NULL)
			{
				symtab_addsymbol(symtab, p);
				p = p->nextsame;
			}
		}
	}
}

void symtab_enterscope(struct Symtab* symtab)
{
	symtab->scope_count++;
	if (++symtab->scope_len >= symtab->scope_size)
	{
		symtab->scope_size += SCOPE_EXPAND;
		int* old_scope = symtab->scope;
		symtab->scope = (int*)malloc(symtab->scope_size * sizeof(int));
		CHECK_MALLOC(symtab->scope);
		for (int i = 0; i < symtab->scope_size - SCOPE_EXPAND; i++)
		{
			symtab->scope[i] = old_scope[i];
		}
	}
	symtab->scope[symtab->scope_len - 1] = symtab->scope_count;
}

void symtab_leavescope(struct Symtab* symtab)
{
	symtab->scope_len--;
}

struct Symbol* symtab_lookup(struct Symtab* symtab, char* name, int* scope, int scope_size)
{
	int hash = hash_string(name) % symtab->symbol_size;
	struct Symbol* p = symtab->symbols[hash];
	while (p != NULL)
	{
		if (p->scope_size == scope_size&& !strcmp(p->name, name))
		{
			for (int i = 0; i < scope_size; i++)
			{
				if (p->scope[i] != scope[i])
				{
					goto unmatch;
				}
			}
			break;
		}
	unmatch:
		p = p->nextsame;
	}
	if (p == NULL && scope_size - 1 >= 1)
	{
		p = symtab_lookup(symtab, name, scope, scope_size - 1);
	}
	return p;
}

void symtab_print(struct Symtab* symtab)
{
	for (int i = 0; i < symtab->symbol_size; i++)
	{
		struct Symbol* p = symtab->symbols[i];
		while (p != NULL)
		{
			printf("%p", p);
			printf("\t");
			printf("%s", p->name);
			printf("\t");
			for (int i = 0; i < p->scope_size; i++)
			{
				printf("%d/", p->scope[i]);
			}
			printf("\t");
			if (p->type != NULL)
			{
				type_print(p->type);
				if (p->type->typetype != T_FUNC)
				{
					printf("\t");
					printf("%d", p->var.offset);
				}
			}
			printf("\n");
			p = p->nextsame;
		}
	}
}

struct Symbol* symbol_new(struct Symtab* symtab, char* name, struct Type* type, int flag)
{
	struct Symbol* symbol = (struct Symbol*)malloc(sizeof(struct Symbol));
	if (symbol == NULL)
	{
		printf("变量符号表分配内存失败");
		exit(1);
	}
	symbol->flag = flag;
	symbol->name = name;
	symbol->scope_size = symtab->scope_len;
	symbol->scope = (int*)malloc(symtab->scope_len * sizeof(int));
	CHECK_MALLOC(symbol->scope);
	for (int i = 0; i < symtab->scope_len; i++)
	{
		symbol->scope[i] = symtab->scope[i];
	}
	symbol->nextsame = NULL;
	symbol->type = type;

	if (CHECK_FLAG(flag, SF_STRUCT))
	{
		symbol->struc.member_num = 0;
		symbol->struc.members = (struct Symbol**)malloc(1024 * sizeof(struct Symbol*));
		CHECK_MALLOC(symbol->struc.members);
		for (int i = 0; i < 1024; i++)symbol->struc.members[i] = NULL;
		symbol->struc.size = 0;
	}

	if (type == NULL)
	{
		goto end;
	}
	if (type->typetype == T_FUNC)
	{
		symbol->func.arg_num = 0;
		symbol->func.args = (struct Symbol**)malloc(1024 * sizeof(struct Symbol*));
		CHECK_MALLOC(symbol->func.args);
		for (int i = 0; i < 1024; i++)symbol->func.args[i] = NULL;
		symbol->func.curesp = 0;
		symbol->func.scopeesp = 0;
		symbol->func.argoff = -4;
	}
	else if (CHECK_FLAG(flag, SF_MEMBER))
	{
		struct Symbol* structsym = symtab->curstruct;
		int size = type_getsize(symbol->type);
		symbol->var.offset = structsym->struc.size;
		structsym->struc.size += size;
		structsym->struc.members[structsym->struc.member_num++] = symbol;

		struct Type* structtype = structsym->type;
		if (structtype->child[0] == NULL)
		{
			structtype->child[0] = type;
		}
		else
		{
			struct Type* p = structtype->child[0];
			while (p->sibling != NULL)
			{
				p = p->sibling;
			}
			p->sibling = type;
			type->last_sibling = p;
		}
	}
	else
	{
		struct Symbol* funcsym = symtab->curfunc;
		if (funcsym != NULL && !CHECK_FLAG(flag, SF_CONST))
		{
			int size = type_getsize(symbol->type);
			if (CHECK_FLAG(flag, SF_ARG))
			{
				funcsym->func.args[funcsym->func.arg_num++] = symbol;
				funcsym->func.argoff -= size;
				symbol->var.offset = funcsym->func.argoff;
				struct Type* functype = funcsym->type;
				if (functype->child[1] == NULL)
				{
					functype->child[1] = type;
				}
				else
				{
					struct Type* p = functype->child[1];
					while (p->sibling != NULL)
					{
						p = p->sibling;
					}
					p->sibling = type;
					type->last_sibling = p;
				}
			}
			else
			{
				funcsym->func.scopeesp += size;
				funcsym->func.curesp += size;
				symbol->var.offset = funcsym->func.curesp;
			}
		}
		else if (CHECK_FLAG(flag, SF_CONST))
		{
			symbol->scope_size = 1;
		}
	}
end:
	return symbol;
}

int hash_string(char* str)
{
	unsigned int hash = 0;
	int hash_limit = 2147483647;
	int base = 97;
	while (*str != '\0')
	{
		hash = (hash * base + *str) % hash_limit;
		str++;
	}
	return hash;
}

void symtab_typecheck(struct Symtab* symtab, struct AST* ast)
{
	while (ast != NULL)
	{
		switch (ast->nodetype)
		{
		case NT_FUNCDEF:
		{
			char* name = ast->attr.sval;
			struct Type* type = type_new(T_FUNC);
			type->child[0] = ast->type;
			struct Symbol* symbol = symbol_new(symtab, name, type, symtab->globeflag);

			symbol->anname = (char*)malloc(sizeof(char) * (strlen(ast->attr.sval) + 1));
			CHECK_MALLOC(symbol->anname);
			symbol->anname[0] = '_';
			symbol->anname[1] = '\0';
			symbol->anname = strcat(symbol->anname, ast->attr.sval);

			symtab_addsymbol(symtab, symbol);

			symtab->curfunc = symbol;
			symtab_enterscope(symtab);

			symtab->globeflag |= SF_ARG;
			symtab_typecheck(symtab, ast->child[0]);
			symtab->globeflag ^= SF_ARG;
			symtab_typecheck(symtab, ast->child[1]);

			symtab_leavescope(symtab);
			symtab->curfunc = NULL;
			break;
		}
		case NT_COMPOUNDSTMT:
			symtab_enterscope(symtab);
			symtab_typecheck(symtab, ast->child[0]);
			symtab_leavescope(symtab);
			break;
		case NT_ARGDEF:
		{
			char* name = ast->attr.sval;
			struct Symbol* symbol = symbol_new(symtab, name, ast->type, SF_ARG);
			symtab_addsymbol(symtab, symbol);
			break;
		}
		case NT_VARDEF:
		{
			char* name = ast->attr.sval;
			int flag = symtab->globeflag;
			if (ast->type->typetype == T_STRUCT)
			{
				struct Symbol* structsym = symtab_lookup(symtab, ast->type->attr.sval, symtab->scope, symtab->scope_len);
				ast->type = structsym->type;
				flag = 0;
			}
			struct Symbol* symbol = symbol_new(symtab, name, ast->type, flag);
			symtab_addsymbol(symtab, symbol);

			if (ast->child[0] != NULL)
			{
				struct AST* t = ast_new2(NT_BINOP);
				t->attr.sval = "=";
				t->child[0] = ast_new2(NT_NAME);
				t->child[0]->type = ast->type;
				t->child[0]->attr.sval = ast->attr.sval;
				t->child[1] = ast->child[0];
				ast->child[0] = t;
			}
			break;
		}
		case NT_STRING:
		{
			char* name = ast->attr.sval;
			if (ast->type == NULL)
			{
				ast->type = type_new(T_ARRAY);
				ast->type->child[0] = type_new(T_CHAR);
				ast->type->attr.ival = strlen(name);
			}
			struct Symbol* symbol = symbol_new(symtab, name, ast->type, SF_CONST);
			symtab_addsymbol(symtab, symbol);
			break;
		}
		case NT_CHAR:
		{
			ast->type = type_new(T_CHAR);
			break;
		}
		case NT_EXTERNSTMT:
		{
			//extern修饰的都是全局变量
			int scope_len = symtab->scope_len;
			symtab->scope_len = 1;
			symtab->globeflag |= SF_EXTERN;
			symtab_typecheck(symtab, ast->child[0]);
			symtab->globeflag ^= SF_EXTERN;
			symtab->scope_len = scope_len;
			break;
		}
		case NT_NAME:
		{
			struct Symbol* sym = symtab_lookup(symtab, ast->attr.sval, symtab->scope, symtab->scope_len);
			if (sym == NULL)
			{
				ast_error(ast, "Undefined identifier");
			}
			ast->type = sym->type;
			break;
		}
		case NT_SUBSCRIPT:
		{
			symtab_typecheck(symtab, ast->child[0]);
			ast->type = ast->child[0]->type->child[0];
			if (ast->child[0]->type->typetype == T_POINTER || ast->child[0]->type->typetype == T_ARRAY)
			{
				struct AST* t = ast_new2(NT_UNARYOP);
				t->attr.sval = "*";
				t->child[0] = ast->child[0];
				symtab_typecheck(symtab, t);
				ast->child[0] = t;
			}
			symtab_typecheck(symtab, ast->child[1]);
			break;
		}
		case NT_CALL:
		{
			symtab_typecheck(symtab, ast->child[0]);
			symtab_typecheck(symtab, ast->child[1]);

			struct Type* tt = ast->child[0]->type->child[1];
			if (ast->child[1] != NULL && tt != NULL)
			{
				//将数组强制转换成指针
				if (ast->child[1]->type->typetype == T_ARRAY)
				{
					struct AST* t = ast_new2(NT_IMPLICITCAST);
					t->filename = ast->child[1]->filename;
					t->line = ast->child[1]->line;
					t->linecode = ast->child[1]->linecode;
					t->start_column = ast->child[1]->start_column;
					t->column = ast->child[1]->column;

					t->type = type_new(T_POINTER);
					t->type->child[0] = ast->child[1]->type->child[0];
					t->child[0] = ast->child[1];
					t->sibling = ast->child[1]->sibling;
					ast->child[1]->sibling = NULL;
					ast->child[1] = t;
				}
				else if (ast->child[1]->type->typetype != tt->typetype)
				{
					struct AST* t = ast_new2(NT_IMPLICITCAST);
					t->filename = ast->child[1]->filename;
					t->line = ast->child[1]->line;
					t->linecode = ast->child[1]->linecode;
					t->start_column = ast->child[1]->start_column;
					t->column = ast->child[1]->column;

					t->type = tt;
					t->child[0] = ast->child[1];
					t->sibling = ast->child[1]->sibling;
					ast->child[1]->sibling = NULL;
					ast->child[1] = t;
				}
				struct AST* p = ast->child[1];
				tt = tt->sibling;
				while (p->sibling != NULL)
				{
					if (!CHECK_FLAG(ast->child[0]->type->attribute, TF_INDEFARG) && (tt == NULL || tt->sibling == NULL))
					{
						ast_error(p->sibling, "Too much args");
					}
					struct AST* s = p->sibling;
					if (s->type->typetype == T_ARRAY)
					{
						struct AST* t = ast_new2(NT_IMPLICITCAST);
						t->filename = s->filename;
						t->line = s->line;
						t->linecode = s->linecode;
						t->start_column = s->start_column;
						t->column = s->column;

						t->type = type_new(T_POINTER);
						t->type->child[0] = s->type->child[0];
						t->child[0] = s;
						t->sibling = s->sibling;
						s->sibling = NULL;
						s->last_sibling = NULL;
						p->sibling = t;
					}
					else if (tt != NULL && ast->child[1]->type->typetype != tt->typetype)
					{
						struct AST* t = ast_new2(NT_IMPLICITCAST);
						t->filename = s->filename;
						t->line = s->line;
						t->linecode = s->linecode;
						t->start_column = s->start_column;
						t->column = s->column;

						t->type = tt;
						t->child[0] = s;
						t->sibling = s->sibling;
						s->sibling = NULL;
						s->last_sibling = NULL;
						p->sibling = t;
					}
					p->sibling->last_sibling = p;
					p = p->sibling;
					if (tt != NULL)
					{
						tt = tt->sibling;
					}
				}
				if (!CHECK_FLAG(ast->child[0]->type->attribute, TF_INDEFARG) && tt != NULL)
				{
					ast_error(ast, "Too few args");
				}
			}
			if (ast->child[0]->type->typetype != T_FUNC)
			{
				ast_error(ast->child[0], "Cannot call no function");
			}
			ast->type = ast->child[0]->type->child[0];
			break;
		}
		case NT_BINOP:
		{
			symtab_typecheck(symtab, ast->child[0]);
			symtab_typecheck(symtab, ast->child[1]);
			ast->type = ast->child[0]->type;
			break;
		}
		case NT_UNARYOP:
		{
			if (!strcmp(ast->attr.sval, "&"))
			{
				symtab_typecheck(symtab, ast->child[0]);
				ast->type = type_new(T_POINTER);
				ast->type->child[0] = ast->child[0]->type;
			}
			else if (!strcmp(ast->attr.sval, "*"))
			{
				symtab_typecheck(symtab, ast->child[0]);
				if (ast->child[0]->type->typetype != T_POINTER && ast->child[0]->type->typetype != T_ARRAY)
				{
					ast_error(ast->child[0], "Cannot unpack no pointer");
				}
				ast->type = ast->child[0]->type->child[0];
			}
			break;
		}
		case NT_STRUCTDEF:
		{
			ast->type = type_new(T_STRUCT);
			ast->type->attr.sval = ast->attr.sval;
			struct Symbol* symbol = symbol_new(symtab, ast->attr.sval, ast->type, symtab->globeflag | SF_STRUCT);
			symtab_addsymbol(symtab, symbol);

			symtab->curstruct = symbol;
			symtab->globeflag |= SF_MEMBER;
			symtab_enterscope(symtab);
			symtab_typecheck(symtab, ast->child[0]);
			symtab_leavescope(symtab);
			symtab->curstruct = NULL;
			symtab->globeflag ^= SF_MEMBER;
			break;
		}
		case NT_MEMBER:
		{
			symtab_typecheck(symtab, ast->child[0]);

			struct Symbol* sym = symtab_lookup(symtab, ast->child[0]->type->attr.sval, symtab->scope, symtab->scope_len);
			for (int i = 0; i < sym->struc.member_num; i++)
			{
				if (!strcmp(sym->struc.members[i]->name, ast->attr.sval))
				{
					ast->type = sym->struc.members[i]->type;
					break;
				}
			}
			break;
		}
		case NT_ELLIPSIS:
			if (CHECK_FLAG(symtab->globeflag, SF_ARG))
			{
				struct Symbol* funcsym = symtab->curfunc;
				funcsym->type->attribute |= TF_INDEFARG;
			}
			break;
		default:
			for (int i = 0; i < MAX_ASTCHILD_NUM; i++)
			{
				if (ast->child[i] != NULL)
				{
					symtab_typecheck(symtab, ast->child[i]);
				}
			}
		}
		ast = ast->sibling;
	}
}