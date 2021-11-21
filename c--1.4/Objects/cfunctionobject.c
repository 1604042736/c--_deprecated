#include "cfunctionobject.h"

struct Object* CFunctionObject_New()
{
	struct CFunctionObject* ct = (struct CFunctionObject*)malloc(sizeof(struct CFunctionObject));
	ct->objattr = &CFunctionObjectAttribute;
	ct->name = "";
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