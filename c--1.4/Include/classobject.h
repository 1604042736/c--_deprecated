#pragma once

#include "object.h"
#include "listobject.h"
#include "stringobject.h"
#include "dictobject.h"

struct ClassObject
{
	OBJECT_HEAD;
	struct StringObject* name;
	struct ListObject* bases;
	struct DictObject* dict;
};

struct Object* ClassObject_New();
void ClassObject_Print(struct Object*);
struct Object* ClassObject_Call(struct Object*, struct Object*);

static struct ObjectAttribute ClassObjectAttribute = {
	(char*)"class",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	ClassObject_Call,	//obj_call
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
	ClassObject_New,	//obj_new
	NULL,	//obj_or
	ClassObject_Print,	//obj_print
	NULL,	//obj_tostring
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct InstanceObject
{
	OBJECT_HEAD;
	struct StringObject* classname;
};

struct Object* InstanceObject_New();
void InstanceObject_Print(struct Object*);
struct Object* InstanceObject_GetAttr(struct Object*, struct Object*);
void InstanceObject_SetAttr(struct Object*, struct Object*, struct Object*);

static struct ObjectAttribute InstanceObjectAttribute = {
	(char*)"instance",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_call
	NULL,	//obj_copy
	NULL,	//obj_div
	NULL,	//obj_eq
	InstanceObject_GetAttr,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	InstanceObject_New,	//obj_new
	NULL,	//obj_or
	InstanceObject_Print,	//obj_print
	NULL,	//obj_tostring
	InstanceObject_SetAttr,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};