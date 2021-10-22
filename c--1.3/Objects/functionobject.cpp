#include "functionobject.h"
#include "stringobject.h"

Object* FunctionObject_New()
{
	FunctionObject* funcobj = new FunctionObject;
	funcobj->objattr = ObjectAttr_New("Function");
	funcobj->objattr->obj_print = FunctionObject_Print;
	funcobj->objattr->obj_copy = FunctionObject_Copy;
	return (Object*)funcobj;
}

void FunctionObject_Print(Object* obj)
{
	FunctionObject* funcobj = (FunctionObject*)obj;
	cout << "<Function " << ((StringObject*)funcobj->name)->str << " at " << funcobj<<">";
}

Object* FunctionObject_Copy(Object* obj)
{
	FunctionObject* funcobj = (FunctionObject*)obj;
	FunctionObject* copyfunc = (FunctionObject*)FunctionObject_New();
	copyfunc->name = funcobj->name;
	copyfunc->argnames = funcobj->argnames;
	copyfunc->frame = funcobj->frame->objattr->obj_copy(funcobj->frame);
	return (Object*)copyfunc;
}