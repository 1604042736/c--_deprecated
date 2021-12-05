#pragma once

#include "object.h"
#include "listobject.h"

#define BLOCKTYPE_WHILE		0
#define BLOCKTYPE_EXCEPTION	1

struct BlockObject
{
	OBJECT_HEAD;
	int type;
	struct ListObject* flags;
};

struct Object* BlockObject_New();

static struct ObjectAttribute BlockObjectAttribute = {
	(char*)"block",	//obj_name
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
	NULL,	//obj_hash
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	BlockObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_tostring
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};