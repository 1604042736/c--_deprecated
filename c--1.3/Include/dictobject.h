#pragma once

#include <map>
#include "object.h"
#include "listobject.h"

struct DictObject
{
	OBJECT_HEAD;
	map<Object*, Object*> dict;
};

Object* DictObject_New();
Object* DictObject_NewSimple();
void DictObject_Print(Object*);
Object* DictObject_GetItem(Object*, Object*);
void DictObject_DictAddItem(Object*, Object*,Object*);
Object* DictObject_Copy(Object*);

Object* DictObjectDict_GetItem(Object*);
Object* DictObjectDict_DictAddItem(Object*);
Object* DictObjectDict_Copy(Object*);

static ObjectAttr DictObjectAttr = {
	"Dict",	//obj_name
	DictObject_Print,	//obj_print
	NULL,	//obj_additem
	NULL,	//obj_find
	DictObject_GetItem,	//obj_getitem
	DictObject_DictAddItem,	//obj_dictadditem
	NULL,	//obj_insertitem
	DictObject_DictAddItem,	//obj_setitem
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
	DictObject_Copy,	//obj_copy
	NULL	//dict
};