#pragma once

#include <string.h>
#include "object.h"
#include "listobject.h"
#include "intobject.h"
#include "dictobject.h"

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
struct Object* StringObject_Eq(struct Object*, struct Object*);
struct ListObject* StringObject_Split(struct Object*, struct Object*);
int StringObject_Bool(struct Object*);
struct Object* StringObject_Format(struct Object*, struct Object*);
struct Object* StringObject_ToString(struct Object*);
int StringObject_Hash(struct Object*);

struct Object* StringObject_Add2(struct Object*);
struct Object* StringObject_Eq2(struct Object*);
struct Object* StringObject_Split2(struct Object*);
struct Object* StringObject_ToString2(struct Object*);

unsigned int DJBHash(char* str);

struct DictObject* stringobjattr;
static struct ObjectAttribute StringObjectAttribute = {
	(char*)"string",	//obj_name
	StringObject_Add,	//obj_add
	NULL,	//obj_and
	StringObject_Bool,	//obj_bool
	NULL,	//obj_call
	NULL,	//obj_copy
	NULL,	//obj_div
	StringObject_Eq,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	StringObject_Hash,	//obj_hash
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	StringObject_New,	//obj_new
	NULL,	//obj_or
	StringObject_Print,	//obj_print
	StringObject_ToString,	//obj_tostring
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct StringObject* conststr_add;
struct StringObject* conststr_and;
struct StringObject* conststr_bool;
struct StringObject* conststr_call;
struct StringObject* conststr_copy;
struct StringObject* conststr_div;
struct StringObject* conststr_eq;
struct StringObject* conststr_getattr;
struct StringObject* conststr_getitem;
struct StringObject* conststr_geq;
struct StringObject* conststr_gt;
struct StringObject* conststr_hash;
struct StringObject* conststr_insertitem;
struct StringObject* conststr_leq;
struct StringObject* conststr_lt;
struct StringObject* conststr_mod;
struct StringObject* conststr_mul;
struct StringObject* conststr_neq;
struct StringObject* conststr_new;
struct StringObject* conststr_or;
struct StringObject* conststr_print;
struct StringObject* conststr_tostring;
struct StringObject* conststr_setattr;
struct StringObject* conststr_setitem;
struct StringObject* conststr_sub;
struct StringObject* conststr_includepath;