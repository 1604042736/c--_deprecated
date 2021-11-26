#include "functionobject.h"
#include "eval.h"

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

struct Object* FunctionObject_Call(struct Object* self, struct Object* arg)
{
	struct FunctionObject* func = (struct FunctionObject*)self;
	struct ListObject* args = (struct ListObject*)arg;
	struct FrameObject* frameobj = FrameObject_NewWithByteCode(func->bytecode);
	frameobj->argindex = args->size - 1;
	for (int i = 0; i < args->size; i++)
	{
		ListObject_InsertItem(frameobj->statck, frameobj->statck->size, args->item[i]);
	}
	struct DictObject* dict = DictObject_New();
	struct FrameObject* frame = func->frame;
	for (int i = 0; i < frame->globals->size; i++)
	{
		DictObject_SetItem(dict, frame->globals->item[i].key, frame->globals->item[i].value);
	}
	for (int i = 0; i < frame->locals->size; i++)
	{
		DictObject_SetItem(dict, frame->locals->item[i].key, frame->locals->item[i].value);
	}
	frameobj->globals = dict;
	return Eval(frameobj);
}