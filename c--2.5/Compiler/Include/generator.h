#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "symtab.h"
#include "ast.h"
#include "lexer.h"

#define BOTHPRINT(a,b,...) fprintf(a,b,__VA_ARGS__);printf(b,__VA_ARGS__);

//代码生成器
struct Generator
{
	FILE* file;
	int label_count;
	int const_count;

	int scope_len;	//作用域长度
	int scope_size;	//scope总长度
	int* scope;	//作用域
	int scope_count;	//作用域计数

	int loop_start_label, loop_end_label;
};

struct Generator* generator_new(char*);
void generator_generate(struct Generator*, struct Symtab*, struct AST*);
void generator_generate_data(struct Generator*, struct Symtab*);
void generator_generate_text(struct Generator*, struct Symtab*, struct AST*);
void _generator_generate_text(struct Generator*, struct Symtab*,struct AST*);
void generator_single(struct Generator*, struct Symtab*, struct AST*);
int generator_getlabel(struct Generator*);
void generator_writelabel(struct Generator*,int);
void generator_writelabel2(struct Generator*, char*);
void generator_write0(struct Generator*, char*);
void generator_write1(struct Generator*, char*, char*);
void generator_write2(struct Generator*, char*, char*, char*);
char* generator_getlabelstr(struct Generator*, int);
void generator_enterscope(struct Generator*);
void generator_leavescope(struct Generator*);