#pragma once

#include "object.h"

struct FunctionObject
{
	OBJECT_HEAD;
	Object* frame;
	Object* name;
	Object* argnames;
};

Object* FunctionObject_New();
void FunctionObject_Print(Object*);
Object* FunctionObject_Copy(Object*);