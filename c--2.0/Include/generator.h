#pragma once

#include <stdio.h>
#include <stdlib.h>

#define MAXINSLEN 1024

/*÷∏¡Ó*/
struct Ins
{
	char* op;
	char* arg1;
	char* arg2;
};

struct _Code
{
	int nowpos;
	struct Ins* ins[MAXINSLEN];
	struct _Code* next;
};

struct Code
{
	struct _Code* nowcode;
	struct _Code* allcode;
};

/*∂Œ*/
struct Section
{
	char* name;
	struct Code* code;
};

struct Ins* ins_init(char* op, char* arg1, char* arg2);
struct _Code* _code_init();
struct Code* code_init();
void code_findempty(struct Code* code);
void code_add(struct Code* code, struct Ins* ins);
struct Section* section_init(char* name);