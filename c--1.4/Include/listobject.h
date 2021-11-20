#pragma once

#include <string.h>
#include "object.h"

struct ListObject
{
	OBJECT_HEAD;
	int size;
	int allocated;
	struct Object** item;
};

struct Object* ListObject_New();
void ListObject_Print(struct Object*);
void ListObject_InsertItem(struct Object*, int,struct Object*);
void ListObject_SetItem(struct Object*, struct Object*, struct Object*);
int ListObject_FindItem(struct Object*, struct Object*);
void ListObject_ListDelItem(struct Object*, int);
int ListObject_Bool(struct Object*);

static struct ObjectAttribute ListObjectAttribute = {
	(char*)"list",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	ListObject_Bool,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_geq
	NULL,	//obj_gt
	ListObject_InsertItem,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	ListObject_New,	//obj_new
	NULL,	//obj_or
	ListObject_Print,	//obj_print
	ListObject_SetItem,	//obj_setitem
	NULL,	//obj_sub
};