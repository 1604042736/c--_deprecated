#pragma once

#include <vector>
using namespace std;

#include "object.h"
#include "intobject.h"

struct ListObject
{
	OBJECT_HEAD;
	vector<Object*> list;
};

Object* ListObject_New();
void ListObject_Print(Object*);
void ListObject_AddItem(Object*,Object*);
int ListObject_FindItem(Object*, Object*);
Object* ListObject_GetItem(Object*, Object*);
void ListObject_InsertItem(Object*, Object*, Object*);
bool ListObject_Bool(Object*);
Object* ListObject_Copy(Object*);
void ListObject_SetItem(Object* ,Object*,Object*);