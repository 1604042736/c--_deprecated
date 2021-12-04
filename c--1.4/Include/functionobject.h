#pragma once

#include "object.h"
#include "bytecodeobject.h"
#include "frameobject.h"

struct FunctionObject
{
	OBJECT_HEAD;
	char* name;
	struct ByteCodeObject* bytecode;
	struct FrameObject* frame;
};

struct Object* FunctionObject_New();
void FunctionObject_Print(struct Object*);
struct Object* FunctionObject_Call(struct Object*, struct Object*);

static struct ObjectAttribute FunctionObjectAttribute = {
	(char*)"function",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	FunctionObject_Call,	//obj_call
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
	FunctionObject_New,	//obj_new
	NULL,	//obj_or
	FunctionObject_Print,	//obj_print
	NULL,	//obj_tostring
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};