#pragma once

#include "object.h"

struct DoubleObject
{
	OBJECT_HEAD;
	double value;
};

struct Object* DoubleObject_New();
struct Object* DoubleObject_NewWithValue(double);
void DoubleObject_Print(struct Object*);
struct Object* DoubleObject_Add(struct Object*, struct Object*);
struct Object* DoubleObject_Eq(struct Object*, struct Object*);
struct Object* DoubleObject_Sub(struct Object*, struct Object*);
struct Object* DoubleObject_Mul(struct Object*, struct Object*);
struct Object* DoubleObject_Div(struct Object*, struct Object*);
struct Object* DoubleObject_Neq(struct Object*, struct Object*);
struct Object* DoubleObject_Gt(struct Object*, struct Object*);
struct Object* DoubleObject_Geq(struct Object*, struct Object*);
struct Object* DoubleObject_Lt(struct Object*, struct Object*);
struct Object* DoubleObject_Leq(struct Object*, struct Object*);
int DoubleObject_Bool(struct Object*);
struct Object* DoubleObject_And(struct Object*, struct Object*);
struct Object* DoubleObject_Or(struct Object*, struct Object*);

static struct ObjectAttribute DoubleObjectAttribute = {
	(char*)"double",	//obj_name
	DoubleObject_Add,	//obj_add
	DoubleObject_And,	//obj_and
	DoubleObject_Bool,	//obj_bool
	NULL,	//obj_call
	NULL,	//obj_copy
	DoubleObject_Div,	//obj_div
	DoubleObject_Eq,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	DoubleObject_Geq,	//obj_geq
	DoubleObject_Gt,	//obj_gt
	NULL,	//obj_insertitem
	DoubleObject_Leq,	//obj_leq
	DoubleObject_Lt,	//obj_lt
	NULL,	//obj_mod
	DoubleObject_Mul,	//obj_mul
	DoubleObject_Neq,	//obj_neq
	DoubleObject_New,	//obj_new
	DoubleObject_Or,	//obj_or
	DoubleObject_Print,	//obj_print
	NULL,	//obj_tostring
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	DoubleObject_Sub,	//obj_sub
};