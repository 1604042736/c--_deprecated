#pragma once

#include "object.h"
#include "listobject.h"
#include "dictobject.h"

#define CheckIntObject(OBJ) !((OBJ)->objattr->obj_name=="Int")

struct IntObject
{
	OBJECT_HEAD;
	int val;
};

Object* IntObject_NewFromInt(int);
void IntObject_Print(Object*);
bool IntObject_Eq(Object*, Object*);
Object* IntObject_Add(Object*, Object*);
Object* IntObject_Sub(Object*, Object*);
Object* IntObject_Mul(Object*, Object*);
Object* IntObject_Div(Object*, Object*);
Object* IntObject_Mod(Object*, Object*);
bool IntObject_Bool(Object*);
bool IntObject_Or(Object*, Object*);
bool IntObject_Gt(Object*, Object*);
bool IntObject_Geq(Object*, Object*);
bool IntObject_Lt(Object*, Object*);
bool IntObject_Leq(Object*, Object*);
bool IntObject_Neq(Object*, Object*);
bool IntObject_And(Object*, Object*);

Object* IntObjectDict_Eq(Object*);
Object* IntObjectDict_Add(Object*);
Object* IntObjectDict_Sub(Object*);
Object* IntObjectDict_Mul(Object*);
Object* IntObjectDict_Div(Object*);
Object* IntObjectDict_Mod(Object*);
Object* IntObjectDict_Bool(Object*);
Object* IntObjectDict_Or(Object*);
Object* IntObjectDict_Gt(Object*);
Object* IntObjectDict_Geq(Object*);
Object* IntObjectDict_Lt(Object*);
Object* IntObjectDict_Leq(Object*);
Object* IntObjectDict_Neq(Object*);
Object* IntObjectDict_And(Object*);

static ObjectAttr IntObjectAttr = {
	"Int",	//obj_name
	IntObject_Print,	//obj_print
	NULL,	//obj_additem
	NULL,	//obj_find
	NULL,	//obj_getitem
	NULL,	//obj_dictadditem
	NULL,	//obj_insertitem
	NULL,	//obj_setitem
	IntObject_Add,	//obj_add
	IntObject_Sub,	//obj_sub
	IntObject_Mul,	//obj_mul
	IntObject_Div,	//obj_div
	IntObject_Mod,	//obj_mod
	IntObject_Gt,	//obj_gt
	IntObject_Geq,	//obj_geq
	IntObject_Lt,
	IntObject_Leq,
	IntObject_Eq,	//obj_eq
	IntObject_Neq,
	IntObject_And,
	IntObject_Or,	//obj_or
	IntObject_Bool,	//obj_bool
	NULL,	//obj_copy
	NULL	//dict
};