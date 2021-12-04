#include "cfunctionobject.h"
#include "listobject.h"
#include "eval.h"

struct Object* CFunctionObject_New()
{
	struct CFunctionObject* ct = (struct CFunctionObject*)malloc(sizeof(struct CFunctionObject));
	ct->objattr = &CFunctionObjectAttribute;
	ct->name = "";
	ct->refcount = DEFAULTREFCOUNT;
	return (struct Object*)ct;
}

struct Object* CFunctionObject_NewWithCFunc(char* name,CFunction cfunction)
{
	struct CFunctionObject* ct = CFunctionObject_New();
	ct->name = name;
	ct->func = cfunction;
	return (struct Object*)ct;
}

void CFunctionObject_Print(struct Object* self)
{
	struct CFunctionObject* ct = (struct CFunctionObject*)self;
	printf("<CFunction %s at %p>", ct->name, ct);
}

struct Object* CFunctionObject_Call(struct Object* self, struct Object* arg)
{
	struct CFunctionObject* func = (struct CFunctionObject*)self;
	struct ListObject* args = (struct ListObject*)arg;
	struct ListObject* cfuncargs = ListObject_New();

	for (int i = 0; i < args->size; i++)
	{
		ListObject_InsertItem(cfuncargs, 0, args->item[i]);
	}
	Memory_Free(memory, args);
	struct Object* val= func->func(cfuncargs);
	Memory_Free(memory, cfuncargs);
	return val;
}