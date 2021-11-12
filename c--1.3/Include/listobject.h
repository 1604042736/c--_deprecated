#pragma once

#include <vector>
using namespace std;

#include "object.h"
#include "intobject.h"
#include "cfunctionobject.h"
#include "dictobject.h"

#define CheckListObject(OBJ) !((OBJ)->objattr->obj_name=="List")

struct ListObject
{
	OBJECT_HEAD;
	vector<Object*> list;
};

Object* ListObject_New();
Object* ListObject_NewSimple();
void ListObject_Print(Object*);
void ListObject_AddItem(Object*,Object*);
int ListObject_FindItem(Object*, Object*);
Object* ListObject_GetItem(Object*, Object*);
void ListObject_InsertItem(Object*, Object*, Object*);
bool ListObject_Bool(Object*);
Object* ListObject_Copy(Object*);
void ListObject_SetItem(Object* ,Object*,Object*);

Object* ListObjectDict_AddItem(Object*);
Object* ListObjectDict_FindItem(Object*);
Object* ListObjectDict_GetItem(Object*);
Object* ListObjectDict_InsertItem(Object*);
Object* ListObjectDict_Bool(Object*);
Object* ListObjectDict_Copy(Object*);
Object* ListObjectDict_SetItem(Object*);

static ObjectAttr ListObjectAttr = {
	"List",	//obj_name
	ListObject_Print,	//obj_print
	ListObject_AddItem,	//obj_additem
	ListObject_FindItem,	//obj_find
	ListObject_GetItem,	//obj_getitem
	NULL,	//obj_dictadditem
	ListObject_InsertItem,	//obj_insertitem
	ListObject_SetItem,	//obj_setitem
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
	ListObject_Bool,	//obj_bool
	ListObject_Copy,	//obj_copy
	NULL	//dict
};