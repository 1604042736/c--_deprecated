#pragma once

#include "object.h"
#include "codeobject.h"
#include "listobject.h"
#include "dictobject.h"
#include "builtins.h"

struct FrameObject
{
	OBJECT_HEAD;
	Object* codeobj;	//代码
	Object* builtins;	//内建
	Object* globals;	//全局作用域
	Object* locals;		//局部作用域
	Object* stack;	//栈
};

Object* FrameObject_NewFrameCodeObject(Object*,Object*);
Object* FrameObject_Copy(Object*);

static ObjectAttr FrameObjectAttr = {
	"Frame",	//obj_name
	NULL,	//obj_print
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
	FrameObject_Copy,	//obj_copy
	NULL	//dict
};