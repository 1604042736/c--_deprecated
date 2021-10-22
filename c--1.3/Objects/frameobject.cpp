#include "frameobject.h"

Object* FrameObject_NewFrameCodeObject(Object* obj)
{
	FrameObject* frameobj = new FrameObject;
	frameobj->codeobj = obj;
	frameobj->builtins = builtins_init();
	frameobj->globals = DictObject_New();
	frameobj->locals = DictObject_New();
	frameobj->stack = ListObject_New();
	frameobj->objattr = ObjectAttr_New("Frame");
	frameobj->objattr->obj_copy = FrameObject_Copy;
	return (Object*)frameobj;
}

Object* FrameObject_Copy(Object* obj)
{
	FrameObject* frameobj = (FrameObject*)obj;
	FrameObject* copyobj = (FrameObject*)FrameObject_NewFrameCodeObject(frameobj->codeobj);
	copyobj->globals = frameobj->globals;
	copyobj->locals = frameobj->locals->objattr->obj_copy(frameobj->locals);
	return (Object*)copyobj;
}