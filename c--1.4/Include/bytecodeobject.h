#pragma once

#include "object.h"
#include "listobject.h"
#include "opcodeobject.h"

struct ByteCodeObject
{
	OBJECT_HEAD;
	char* name;
	struct ListObject* code;
	struct ListObject* consts;
};

struct Object* ByteCodeObject_New();
void ByteCodeObject_Print(struct Object*);
void ByteCodeObject_Print2(struct Object*);
struct Object* ByteCodeObject_Call(struct Object*, struct Object*);

static struct ObjectAttribute ByteCodeObjectAttribute = {
	(char*)"bytecode",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	ByteCodeObject_Call,	//obj_call
	NULL,	//obj_copy
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
	ByteCodeObject_New,	//obj_new
	NULL,	//obj_or
	ByteCodeObject_Print,	//obj_print
	NULL,	//obj_tostring
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};