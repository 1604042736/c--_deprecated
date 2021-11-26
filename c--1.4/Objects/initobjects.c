#include "object.h"
#include "listobject.h"
#include "stringobject.h"
#include "cfunctionobject.h"
#include "intobject.h"

void Object_Init()
{
#define ADD(ATTR,NAME,FUNC)	DictObject_SetItem(ATTR, StringObject_NewWithString(NAME), CFunctionObject_NewWithCFunc(NAME, FUNC));
	listobjectattr = DictObject_New();
	ADD(listobjectattr, "insert", ListObject_InsertItem2);
	ADD(listobjectattr, "remove", ListObject_ListDelItem2);

	intobjectattr = DictObject_New();
	ADD(intobjectattr, "add", IntObject_Add2);
	ADD(intobjectattr, "sub", IntObject_Sub2);
	ADD(intobjectattr, "mul", IntObject_Mul2);
	ADD(intobjectattr, "div", IntObject_Div2);
	ADD(intobjectattr, "mod", IntObject_Mod2);
	ADD(intobjectattr, "eq", IntObject_Eq2);
	ADD(intobjectattr, "neq", IntObject_Neq2);
	ADD(intobjectattr, "lt", IntObject_Lt2);
	ADD(intobjectattr, "leq", IntObject_Leq2);
	ADD(intobjectattr, "bool", IntObject_Bool2);
	ADD(intobjectattr, "and", IntObject_And2);
	ADD(intobjectattr, "or", IntObject_Or2);

	stringobjattr = DictObject_New();
	ADD(stringobjattr, "add", StringObject_Add2);
	ADD(stringobjattr, "split", StringObject_Split2);
	ADD(stringobjattr, "eq", StringObject_Add2);
}