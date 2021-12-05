#pragma once

#include "object.h"

struct CFunctionObject
{
	OBJECT_HEAD;
	char* name;
	CFunction func;
};

struct Object* CFunctionObject_New();
struct Object* CFunctionObject_NewWithCFunc(char*,CFunction);
void CFunctionObject_Print(struct Object*);
struct Object* CFunctionObject_Call(struct Object*, struct Object*);

static struct ObjectAttribute CFunctionObjectAttribute = {
	(char*)"cfunciton",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	CFunctionObject_Call,	//obj_call
	NULL,	//obj_copy
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_hash
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	CFunctionObject_New,	//obj_new
	NULL,	//obj_or
	CFunctionObject_Print,	//obj_print
	NULL,	//obj_tostring
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};