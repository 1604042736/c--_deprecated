#pragma once

#include "object.h"
#include "stringobject.h"

#define EXCEPTION_OBJECT_HEAD	OBJECT_HEAD;\
								int lineno,linepos;\
								struct StringObject* filename;\
								struct StringObject* message

struct ExceptionObject
{
	EXCEPTION_OBJECT_HEAD;
};

struct Object* ExceptionObject_New();
struct Object* ExceptionObject_NewWithMessage(struct StringObject*,struct StringObject*,int,int);
void ExceptionObject_Print(struct Object*);

static struct ObjectAttribute ExceptionObjectAttribute = {
	(char*)"exception",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_call
	NULL,	//obj_copy
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	ExceptionObject_New,	//obj_new
	NULL,	//obj_or
	ExceptionObject_Print,	//obj_print
	NULL,	//obj_tostring
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct ExceptionObject* exception;