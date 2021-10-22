#include "codeobject.h"
#include "listobject.h"
#include "stringobject.h"
#include "intobject.h"
#include "opcode.h"

Object* CodeObject_New()
{
	CodeObject* codeobj = new CodeObject;
	codeobj->names = ListObject_New();
	codeobj->consts = ListObject_New();
	codeobj->code = ListObject_New();
	codeobj->objattr = ObjectAttr_New("Code");
	codeobj->objattr->obj_print = CodeObject_Print;
	return (Object*)codeobj;
}

void CodeObject_Print(Object* obj)
{
	CodeObject* codeobj = (CodeObject*)obj;
	cout << "<CodeObject at " << obj<<">";
}