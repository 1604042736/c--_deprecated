#pragma once

#include <map>
#include "object.h"

struct DictObject
{
	OBJECT_HEAD;
	map<Object*, Object*> dict;
};

Object* DictObject_New();
void DictObject_Print(Object*);
Object* DictObject_GetItem(Object*, Object*);
void DictObject_DictAddItem(Object*, Object*,Object*);
Object* DictObject_Copy(Object*);