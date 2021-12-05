#pragma once
#include <stdio.h>
#include <string.h>

#include "memory.h"

#pragma warning(disable:4996)

#define OBJECT_HEAD struct ObjectAttribute* objattr;\
					struct Block* block;\
					int refcount
#define MAXALLOCATEDSIZE 256
#define CHECK(OBJ,TYPE) (!strcmp((OBJ)->objattr->obj_name,TYPE))
#define NORMALRETURN	return IntObject_NewWithValue(0);
#define DEFAULTREFCOUNT	0

#define SUBREFCOUNT(OBJ) \
		(OBJ)->refcount--; \
		if ((OBJ)->refcount <= 0)\
			Memory_Free(memory, OBJ);

#define ADDREFCOUNT(OBJ)	(OBJ)->refcount++

#define GETATTR(NAME) \
	struct Object* name = StringObject_NewWithString(NAME);\
	struct Object* func = DictObject_GetItem(self->objattr->attr, name);\
	Memory_Free(memory, name);

struct Object;
typedef struct Object*	(*Add)(struct Object*, struct Object*);
typedef struct Object*	(*And)(struct Object*, struct Object*);
typedef int				(*Bool)(struct Object*);
typedef struct Object*	(*Call)(struct Object*, struct Object*);
typedef struct Object*	(*Copy)(struct Object*);
typedef struct Object*	(*Div)(struct Object*, struct Object*);
typedef struct Object*	(*Eq)(struct Object*, struct Object*);
typedef struct Object*	(*GetAttr)(struct Object*, struct Object*);
typedef struct Object*	(*GetItem)(struct Object*, struct Object*);
typedef struct Object*	(*Geq)(struct Object*, struct Object*);
typedef struct Object*	(*Gt)(struct Object*, struct Object*);
typedef int				(*Hash)(struct Object*);
typedef void			(*InsertItem)(struct Object*, int, struct Object*);
typedef struct Object*	(*Leq)(struct Object*, struct Object*);
typedef struct Object*	(*Lt)(struct Object*, struct Object*);
typedef struct Object*	(*Mod)(struct Object*, struct Object*);
typedef struct Object*	(*Mul)(struct Object*, struct Object*);
typedef struct Object*	(*Neq)(struct Object*, struct Object*);
typedef struct Object*	(*New)();
typedef struct Object*	(*Or)(struct Object*, struct Object*);
typedef void			(*Print)(struct Object*);
typedef struct Object*	(*ToString)(struct Object*);
typedef void			(*SetAttr)(struct Object*, struct Object*, struct Object*);
typedef void			(*SetItem)(struct Object*, struct Object*, struct Object*);
typedef struct Object*	(*Sub)(struct Object*, struct Object*);

typedef struct Object* (*CFunction)(struct Object*);

struct ObjectAttribute
{
	char*		obj_name;	//对象名
	Add			obj_add;	//加法
	And			obj_and;
	Bool		obj_bool;
	Call		obj_call;
	Copy		obj_copy;
	Div			obj_div;
	Eq			obj_eq;	//判断是否相等
	GetAttr		obj_getattr;
	GetItem		obj_getitem;
	Geq			obj_geq;
	Gt			obj_gt;
	Hash		obj_hash;
	InsertItem	obj_insertitem;	//添加元素
	Leq			obj_leq;
	Lt			obj_lt;
	Mod			obj_mod;
	Mul			obj_mul;
	Neq			obj_neq;
	New			obj_new;	//对象创建
	Or			obj_or;
	Print		obj_print;	//对象输出
	ToString	obj_tostring;
	SetAttr		obj_setattr;
	SetItem		obj_setitem;	//设置元素
	Sub			obj_sub;
	struct Object* attr;
};

struct Object
{
	OBJECT_HEAD;
};

void Object_Init();

struct Object*	Object_Add(struct Object*, struct Object*);
struct Object*	Object_And(struct Object*, struct Object*);
int				Object_Bool(struct Object*);
struct Object*	Object_Call(struct Object*, struct Object*);
struct Object*	Object_Div(struct Object*, struct Object*);
struct Object*	Object_Eq(struct Object*, struct Object*);
struct Object*	Object_GetAttr(struct Object*, struct Object*);
struct Object*	Object_GetItem(struct Object*, struct Object*);
struct Object*	Object_Geq(struct Object*, struct Object*);
struct Object*	Object_Gt(struct Object*, struct Object*);
void			Object_InsertItem(struct Object*, int, struct Object*);
struct Object*	Object_Leq(struct Object*, struct Object*);
struct Object*	Object_Lt(struct Object*, struct Object*);
struct Object*	Object_Mod(struct Object*, struct Object*);
struct Object*	Object_Mul(struct Object*, struct Object*);
struct Object*	Object_Neq(struct Object*, struct Object*);
struct Object*	Object_New();
struct Object*	Object_Or(struct Object*, struct Object*);
void			Object_Print(struct Object*);
struct Object*	Object_ToString(struct Object*);
void			Object_SetAttr(struct Object*, struct Object*, struct Object*);
void			Object_SetItem(struct Object*, struct Object*, struct Object*);
struct Object*	Object_Sub(struct Object*, struct Object*);

static struct ObjectAttribute ObjectObjectAttribute = {
	(char*)"object",	//obj_name
	Object_Add,	//obj_add
	Object_And,	//obj_and
	Object_Bool,	//obj_bool
	Object_Call,	//obj_call
	NULL,	//obj_copy
	Object_Div,	//obj_div
	Object_Eq,	//obj_eq
	Object_GetAttr,	//obj_getattr
	Object_GetItem,	//obj_getitem
	Object_Geq,	//obj_geq
	Object_Gt,	//obj_gt
	NULL,	//obj_hash
	Object_InsertItem,	//obj_insertitem
	Object_Leq,	//obj_leq
	Object_Lt,	//obj_lt
	Object_Mod,	//obj_mod
	Object_Mul,	//obj_mul
	Object_Neq,	//obj_neq
	Object_New,	//obj_new
	Object_Or,	//obj_or
	Object_Print,	//obj_print
	Object_ToString,	//obj_tostring
	Object_SetItem,	//obj_setitem
	Object_Sub,	//obj_sub
};