#pragma once

#include "object.h"
#include "dictobject.h"
#include "listobject.h"
#include "bytecodeobject.h"

struct FrameObject
{
	OBJECT_HEAD;
	struct DictObject* globals;
	struct DictObject* locals;
	struct ListObject* statck;
	struct ByteCodeObject* bytecode;
	int argindex;
};

struct Object* FrameObject_New();
struct Object* FrameObject_NewWithByteCode(struct ByteCodeObject*);
void FrameObject_Print(struct Object*);

static struct ObjectAttribute FrameObjectAttribute = {
	(char*)"frame",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_call
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
	FrameObject_New,	//obj_new
	NULL,	//obj_or
	FrameObject_Print,	//obj_print
	NULL,	//obj_tostring
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};