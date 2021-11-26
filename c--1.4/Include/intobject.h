#pragma once

#include "object.h"

struct IntObject
{
	OBJECT_HEAD;
	int value;
};

struct Object* IntObject_New();
struct Object* IntObject_NewWithValue(int);
void IntObject_Print(struct Object*);
struct Object* IntObject_Add(struct Object*, struct Object*);
struct Object* IntObject_Eq(struct Object*, struct Object*);
int IntObject_toInt(struct Object*);
struct Object* IntObject_Sub(struct Object*, struct Object*);
struct Object* IntObject_Mul(struct Object*, struct Object*);
struct Object* IntObject_Div(struct Object*, struct Object*);
struct Object* IntObject_Mod(struct Object*, struct Object*);
struct Object* IntObject_Neq(struct Object*, struct Object*);
struct Object* IntObject_Gt(struct Object*, struct Object*);
struct Object* IntObject_Geq(struct Object*, struct Object*);
struct Object* IntObject_Lt(struct Object*, struct Object*);
struct Object* IntObject_Leq(struct Object*, struct Object*);
int IntObject_Bool(struct Object*);
struct Object* IntObject_And(struct Object*, struct Object*);
struct Object* IntObject_Or(struct Object*, struct Object*);

static struct ObjectAttribute IntObjectAttribute = {
	(char*)"int",	//obj_name
	IntObject_Add,	//obj_add
	IntObject_And,	//obj_and
	IntObject_Bool,	//obj_bool
	NULL,	//obj_call
	IntObject_Div,	//obj_div
	IntObject_Eq,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	IntObject_Geq,	//obj_geq
	IntObject_Gt,	//obj_gt
	NULL,	//obj_insertitem
	IntObject_Leq,	//obj_leq
	IntObject_Lt,	//obj_lt
	IntObject_Mod,	//obj_mod
	IntObject_Mul,	//obj_mul
	IntObject_Neq,	//obj_neq
	IntObject_New,	//obj_new
	IntObject_Or,	//obj_or
	IntObject_Print,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	IntObject_Sub,	//obj_sub
};