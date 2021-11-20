#pragma once

#include <stdlib.h>
#include <stdio.h>

#pragma warning(disable:4996)

#define OBJECT_HEAD struct ObjectAttribute* objattr
#define MAXALLOCATEDSIZE 256
#define CHECK(OBJ,TYPE) (!strcmp((OBJ)->objattr->obj_name,TYPE))

struct Object;
typedef struct Object*	(*Add)(struct Object*, struct Object*);
typedef struct Object*	(*And)(struct Object*, struct Object*);
typedef int				(*Bool)(struct Object*);
typedef struct Object*	(*Div)(struct Object*, struct Object*);
typedef int				(*Eq)(struct Object*, struct Object*);
typedef struct Object*	(*Geq)(struct Oject*, struct Object*);
typedef struct Object*	(*Gt)(struct Object*, struct Object*);
typedef void			(*InsertItem)(struct Object*, int, struct Object*);
typedef struct Object*	(*Leq)(struct Object*, struct Object*);
typedef struct Object*	(*Lt)(struct Object*, struct Object*);
typedef struct Object*	(*Mod)(struct Object*, struct Object*);
typedef struct Object*	(*Mul)(struct Object*, struct Object*);
typedef struct Object*	(*Neq)(struct Object*, struct Object*);
typedef struct Object*	(*New)();
typedef struct Object*	(*Or)(struct Object*, struct Object*);
typedef void			(*Print)(struct Object*);
typedef void			(*SetItem)(struct Object*, struct Object*, struct Object*);
typedef struct Object*	(*Sub)(struct Object*, struct Object*);

struct ObjectAttribute
{
	char*		obj_name;	//������
	Add			obj_add;	//�ӷ�
	And			obj_and;
	Bool		obj_bool;
	Div			obj_div;
	Eq			obj_eq;	//�ж��Ƿ����
	Geq			obj_geq;
	Gt			obj_gt;
	InsertItem	obj_insertitem;	//���Ԫ��
	Leq			obj_leq;
	Lt			obj_lt;
	Mod			obj_mod;
	Mul			obj_mul;
	Neq			obj_neq;
	New			obj_new;	//���󴴽�
	Or			obj_or;
	Print		obj_print;	//�������
	SetItem		obj_setitem;	//����Ԫ��
	Sub			obj_sub;
};

struct Object
{
	OBJECT_HEAD;
};

struct Object* Object_New();
void Object_Print(struct Object*);

static struct ObjectAttribute ObjectObjectAttribute = {
	(char*)"object",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	Object_New,	//obj_new
	NULL,	//obj_or
	Object_Print,	//obj_print
	NULL,	//obj_setitem
	NULL,	//obj_sub
};