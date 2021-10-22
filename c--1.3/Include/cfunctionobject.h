#pragma once

#include "object.h"
#include "stringobject.h"

typedef Object* (*CFunction)(Object*);

struct CFunctionObject
{
	OBJECT_HEAD;
	Object* name;
	CFunction func;
};

Object* CFunctionObject_New(Object*, CFunction);
void CFunctionObject_Print(Object*);