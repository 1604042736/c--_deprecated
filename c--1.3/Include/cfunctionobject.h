#pragma once

#include "object.h"
#include "stringobject.h"

struct CFunctionObject
{
	OBJECT_HEAD;
	Object* name;
	CFunction func;
	Object* self;	//作为方法调用时的参数
};

Object* CFunctionObject_New(Object*, CFunction);
void CFunctionObject_Print(Object*);

static ObjectAttr CFunctionObjectAttr = {
	"CFunction",	//obj_name
	CFunctionObject_Print,	//obj_print
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
	NULL,	//obj_copy
	NULL	//dict
};