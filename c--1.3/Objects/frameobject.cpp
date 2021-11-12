#include "frameobject.h"

Object* FrameObject_NewFrameCodeObject(Object* obj,Object* builtins)
{
	FrameObject* frameobj = new FrameObject;
	frameobj->codeobj = obj;
	frameobj->builtins = builtins;
	frameobj->globals = DictObject_NewSimple();
	frameobj->locals = DictObject_NewSimple();
	frameobj->stack = ListObject_NewSimple();
	frameobj->objattr = &FrameObjectAttr;
	return (Object*)frameobj;
}

Object* FrameObject_Copy(Object* obj)
{
	FrameObject* frameobj = (FrameObject*)obj;
	FrameObject* copyobj = (FrameObject*)FrameObject_NewFrameCodeObject(frameobj->codeobj,frameobj->builtins);
	copyobj->globals = frameobj->globals;
	copyobj->locals = frameobj->locals->objattr->obj_copy(frameobj->locals);
	return (Object*)copyobj;
}