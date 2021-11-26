#pragma once

#include "object.h"

struct DictItem
{
	struct Object* key;
	struct Object* value;
	int hash;	//‘› ±≤ª”√
};

struct DictObject
{
	OBJECT_HEAD;
	int size;
	int allocated;
	struct DictItem* item;
};

struct Object* DictObject_New();
void DictObject_Print(struct Object*);
void DictObject_SetItem(struct Object*, struct Object*, struct Object*);
int DictObject_FindItem(struct Object*, struct Object*);
struct Object* DictObject_GetItem(struct Object*, struct Object*);
int DictObject_Bool(struct Object*);
void DictObject_DelItem(struct Object*, struct Object*);

static struct ObjectAttribute DictObjectAttribute = {
	(char*)"dict",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	DictObject_Bool,	//obj_bool
	NULL,	//obj_call
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	DictObject_GetItem,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	DictObject_New,	//obj_new
	NULL,	//obj_or
	DictObject_Print,	//obj_print
	NULL,	//obj_setattr
	DictObject_SetItem,	//obj_setitem
	NULL,	//obj_sub
};