#include "object.h"
#include "listobject.h"
#include "stringobject.h"
#include "cfunctionobject.h"

void Object_Init()
{
#define ADD(ATTR,NAME,FUNC)	DictObject_SetItem(ATTR, StringObject_NewWithString(NAME), CFunctionObject_NewWithCFunc(NAME, FUNC));
	listobjectattr = DictObject_New();
	ADD(listobjectattr, "insert", ListObject_InsertItem2);
	ADD(listobjectattr, "remove", ListObject_ListDelItem2);
}