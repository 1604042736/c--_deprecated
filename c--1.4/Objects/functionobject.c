#include "functionobject.h"

struct Object* FunctionObject_New()
{
	struct FunctionObject* funcobj = (struct FunctionObject*)malloc(sizeof(struct FunctionObject));
	funcobj->objattr = &FunctionObjectAttribute;
	return (struct Object*)funcobj;
}

void FunctionObject_Print(struct Object* self)
{
	struct FunctionObject* selffunc = (struct FunctionObject*)self;
	printf("<Function %s at %p>", selffunc->name, selffunc);
}