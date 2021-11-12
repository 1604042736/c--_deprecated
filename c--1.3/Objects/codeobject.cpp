#include "codeobject.h"
#include "listobject.h"
#include "stringobject.h"
#include "intobject.h"
#include "opcode.h"

Object* CodeObject_New()
{
	CodeObject* codeobj = new CodeObject;
	codeobj->names = ListObject_NewSimple();
	codeobj->consts = ListObject_NewSimple();
	codeobj->code = ListObject_NewSimple();
	codeobj->objattr = &CodeObjectAttr;
	return (Object*)codeobj;
}

void CodeObject_Print(Object* obj)
{
	CodeObject* codeobj = (CodeObject*)obj;
	cout << "<CodeObject at " << obj<<">";
}