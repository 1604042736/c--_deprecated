#include "cfunctionobject.h"

Object* CFunctionObject_New(Object* name, CFunction func)
{
	CFunctionObject* cfuncobj = new CFunctionObject;
	cfuncobj->name = name;
	cfuncobj->func = func;
	cfuncobj->objattr = ObjectAttr_New("CFunction");
	cfuncobj->objattr->obj_print = CFunctionObject_Print;
	return (Object*)cfuncobj;
}

void CFunctionObject_Print(Object* obj)
{
	CFunctionObject* cfuncobj = (CFunctionObject*)obj;
	cout << "<CFunction " << ((StringObject*)cfuncobj->name)->str << " at " << cfuncobj << ">";
}