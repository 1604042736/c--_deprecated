#pragma once

#include <string.h>
#include "object.h"
#include "listobject.h"
#include "intobject.h"

#define isWhite(ch) (ch==' '||ch=='\n'||ch=='\t')

struct StringObject
{
	OBJECT_HEAD;
	int size;
	char* string;
};

struct Object* StringObject_New();
struct Object* StringObject_NewWithString(char*);
struct Object* StringObject_NewWithChar(char);
void StringObject_Print(struct Object*);
struct Object* StringObject_Add(struct Object*, struct Object*);
int StringObject_Eq(struct Object*, struct Object*);
struct ListObject* StringObject_Split(struct Object*, struct Object*);
int StringObject_Bool(struct Object*);

static struct ObjectAttribute StringObjectAttribute = {
	(char*)"string",	//obj_name
	StringObject_Add,	//obj_add
	NULL,	//obj_and
	StringObject_Bool,	//obj_bool
	NULL,	//obj_div
	StringObject_Eq,	//obj_eq
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	StringObject_New,	//obj_new
	NULL,	//obj_or
	StringObject_Print,	//obj_print
	NULL,	//obj_setitem
	NULL,	//obj_sub
};