#pragma once

#include "object.h"

struct CodeObject
{
	OBJECT_HEAD;
	Object* names;	//名字表
	Object* consts;	//常量表
	Object* code;	//代码
};

Object* CodeObject_New();
void CodeObject_Print(Object*);

static ObjectAttr CodeObjectAttr = {
	"Code",	//obj_name
	CodeObject_Print,	//obj_print
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