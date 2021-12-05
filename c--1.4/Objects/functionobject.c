#include "functionobject.h"
#include "eval.h"

struct Object* FunctionObject_New()
{
	struct FunctionObject* funcobj = (struct FunctionObject*)malloc(sizeof(struct FunctionObject));
	funcobj->objattr = &FunctionObjectAttribute;
	funcobj->refcount = DEFAULTREFCOUNT;
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
	Memory_Free(memory, args);
	struct FrameObject* frame = func->frame;
	for (int i = 0; i < frame->globals->allocated; i++)
	{
		if (frame->globals->item[i].key != NULL)
		{
			DictObject_SetItem(frameobj->globals, frame->globals->item[i].key, frame->globals->item[i].value);
		}
	}
	for (int i = 0; i < frame->locals->allocated; i++)
	{
		if (frame->locals->item[i].key != NULL)
		{
			DictObject_SetItem(frameobj->globals, frame->locals->item[i].key, frame->locals->item[i].value);
		}
	}
	struct Object* val = Eval(frameobj);
	Memory_Free(memory,frameobj);
	return val;
}