#pragma once

#include "object.h"
#include "dictobject.h"

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
struct Object* IntObject_Copy(struct Object*);

struct Object* IntObject_Add2(struct Object*);
struct Object* IntObject_Eq2(struct Object*);
struct Object* IntObject_Sub2(struct Object*);
struct Object* IntObject_Mul2(struct Object*);
struct Object* IntObject_Div2(struct Object*);
struct Object* IntObject_Mod2(struct Object*);
struct Object* IntObject_Neq2(struct Object*);
struct Object* IntObject_Gt2(struct Object*);
struct Object* IntObject_Geq2(struct Object*);
struct Object* IntObject_Lt2(struct Object*);
struct Object* IntObject_Leq2(struct Object*);
struct Object* IntObject_Bool2(struct Object*);
struct Object* IntObject_And2(struct Object*);
struct Object* IntObject_Or2(struct Object*);

struct DictObject* intobjectattr;
static struct ObjectAttribute IntObjectAttribute = {
	"int",	//obj_name
	IntObject_Add,	//obj_add
	IntObject_And,	//obj_and
	IntObject_Bool,	//obj_bool
	NULL,	//obj_call
	IntObject_Copy,	//obj_copy
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
	NULL,	//obj_tostring
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	IntObject_Sub,	//obj_sub
};