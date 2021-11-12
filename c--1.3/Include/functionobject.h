#pragma once

#include "object.h"

struct FunctionObject
{
	OBJECT_HEAD;
	Object* frame;
	Object* name;
	Object* argnames;
};

Object* FunctionObject_New();
void FunctionObject_Print(Object*);
Object* FunctionObject_Copy(Object*);

static ObjectAttr FunctionObjectAttr = {
	"Function",	//obj_name
	FunctionObject_Print,	//obj_print
	NULL,	//obj_additem
	NULL,	//obj_find
	NULL,	//obj_getitem
	NULL,	//obj_dictadditem
	NULL,	//obj_insertitem
	NULL,	//obj_setitem
	NULL,	//obj_add
	NULL,	//obj_sub
	NULL,	//obj_mul
	NULL,	//obj_div
	NULL,	//obj_mod
	NULL,	//obj_gt
	NULL,	//obj_geq
	NULL,	//obj_lt
	NULL,	//obj_leq
	NULL,	//obj_eq
	NULL,	//obj_neq
	NULL,	//obj_add
	NULL,	//obj_or
	NULL,	//obj_bool
	FunctionObject_Copy,	//obj_copy
	NULL	//dict
};