#pragma once

#include<iostream>
#include "object.h"
#include "intobject.h"
#include "listobject.h"

#define CheckStringObject(OBJ) !((OBJ)->objattr->obj_name=="String")

struct StringObject
{
	OBJECT_HEAD;
	string str;
};

Object* StringObject_NewFromString(string);
Object* StringObject_NewFromStringSimple(string);
void StringObject_Print(Object*);
bool StringObject_Eq(Object*,Object*);
Object* StringObject_Add(Object*, Object*);
bool StringObject_Bool(Object*);

Object* StringObjectDict_Eq(Object*);
Object* StringObjectDict_Add(Object*);
Object* StringObjectDict_Bool(Object*);

static ObjectAttr StringObjectAttr = {
	"String",	//obj_name
	StringObject_Print,	//obj_print
	NULL,	//obj_additem
	NULL,	//obj_find
	NULL,	//obj_getitem
	NULL,	//obj_dictadditem
	NULL,	//obj_insertitem
	NULL,	//obj_setitem
	StringObject_Add,	//obj_add
	NULL,	//obj_sub
	NULL,	//obj_mul
	NULL,	//obj_div
	NULL,	//obj_mod
	NULL,	//obj_gt
	NULL,	//obj_geq
	NULL,
	NULL,
	StringObject_Eq,	//obj_eq
	NULL,
	NULL,
	NULL,	//obj_or
	StringObject_Bool,	//obj_bool
	NULL,	//obj_copy
	NULL	//dict
};