#pragma once

#include "object.h"
#include "dictobject.h"

struct NamespaceObject
{
	OBJECT_HEAD;
	struct DictObject* globals;
};

struct Object* NamespaceObject_New();

static struct ObjectAttribute NamespaceObjectAttribute = {
	(char*)"namespace",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
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
	NULL,	//obj_print
	NULL,	//obj_setitem
	NULL,	//obj_sub
};