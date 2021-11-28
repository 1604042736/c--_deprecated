#pragma once

#include "object.h"
#include "stringobject.h"

#define OP_ADD			0
#define OP_SUB			1
#define OP_MUL			3
#define OP_DIV			4
#define OP_MOD			5
#define OP_EQ			6
#define OP_NEQ			7
#define OP_GT			8
#define OP_GEQ			9
#define OP_LT			10
#define OP_LEQ			11
#define OP_RETURN		12
#define OP_OR			13
#define OP_AND			14
#define OP_LOAD_SUB		15
#define OP_STORE_SUB	16
#define OP_GET_TEMP		17
#define OP_SET_TEMP		18
#define OP_POP_TOP		19
#define OP_DEL_BLOCK	20
#define OP_POP_TEMP		21
#define OP_LOAD_LOCAL	22

#define OP_NEW_BLOCK		112
#define OP_BUILD_CLASS		113
#define OP_LOAD_NAMESPACE	114
#define OP_GOTO_FLAG		115
#define OP_ADD_FLAG			116
#define OP_LOAD_ATTR		117
#define OP_STORE_ATTR		118
#define OP_BUILD_LIST		119
#define OP_STORE_ARG		120
#define OP_MAKE_FUNCTION	121
#define OP_CALL				122
#define OP_IFJMP			123
#define OP_JMP				124
#define OP_STORE_NAME		125
#define OP_LOAD_CONST		126
#define OP_LOAD_NAME		127

typedef short opcodetype;

struct OpCodeObject
{
	OBJECT_HEAD;
	opcodetype op;
	opcodetype oparg;
	opcodetype opcode;	//=op<<8+oparg
	int lineno, linepos;
	struct StringObject* filename;
};

struct Object* OpCodeObject_New();
struct Object* OpCodeObject_NewWithInfo(opcodetype, opcodetype, int, int,struct StringObject*);
void OpCodeObject_Print(struct Object*);

static struct ObjectAttribute OpCodeObjectAttribute = {
	(char*)"opcode",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_call
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
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
	NULL,	//obj_tostring
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};