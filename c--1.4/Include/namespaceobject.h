#pragma once

#include "object.h"
#include "dictobject.h"

struct NamespaceObject
{
	OBJECT_HEAD;
	char* name;
	struct DictObject* globals;
};

struct Object* NamespaceObject_New();
struct Object* NamespaceObject_NewWithName(char*);
struct Object* NamespaceObject_GetAttr(struct Object*, struct Object*);
void NamespaceObject_SetAttr(struct Object*, struct Object*,struct Object*);
void NamespaceObject_Print(struct Object*);

static struct ObjectAttribute NamespaceObjectAttribute = {
	(char*)"namespace",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_call
	NULL,	//obj_copy
	NULL,	//obj_div
	NULL,	//obj_eq
	NamespaceObject_GetAttr,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	NamespaceObject_New,	//obj_new
	NULL,	//obj_or
	NamespaceObject_Print,	//obj_print
	NULL,	//obj_tostring
	NamespaceObject_SetAttr,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};