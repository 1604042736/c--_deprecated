#pragma once

#include "object.h"

#define OP_ADD		0
#define OP_SUB		1
#define OP_MUL		3
#define OP_DIV		4
#define OP_MOD		5
#define OP_EQ		6
#define OP_NEQ		7
#define OP_GT		8
#define OP_GEQ		9
#define OP_LT		10
#define OP_LEQ		11
#define OP_RETURN	12
#define OP_OR		13
#define OP_AND		14

#define OP_STORE_ARG		248
#define OP_MAKE_FUNCTION	249
#define OP_CALL				250
#define OP_IFJMP			251
#define OP_JMP				252
#define OP_STORE_NAME		253
#define OP_LOAD_CONST		254
#define OP_LOAD_NAME		255

struct OpCodeObject
{
	OBJECT_HEAD;
	unsigned short op;
	short oparg;
	unsigned short opcode;	//=op<<8+oparg
	int lineno, linepos;
};

struct Object* OpCodeObject_New();
struct Object* OpCodeObject_NewWithOpCodeAndLine(unsigned short, short, int, int);
void OpCodeObject_Print(struct Object*);

static struct ObjectAttribute OpCodeObjectAttribute = {
	(char*)"opcode",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	OpCodeObject_New,	//obj_new
	NULL,	//obj_or
	OpCodeObject_Print,	//obj_print
	NULL,	//obj_setitem
	NULL,	//obj_sub
};