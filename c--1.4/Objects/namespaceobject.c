#include "namespaceobject.h"

struct Object* NamespaceObject_New()
{
	struct NamespaceObject* nt = (struct NamespaceObject*)malloc(sizeof(struct NamespaceObject));
	nt->objattr = &NamespaceObjectAttribute;
	nt->globals = DictObject_New();
	nt->refcount = DEFAULTREFCOUNT;
	return (struct Object*)nt;
}

struct Object* NamespaceObject_NewWithName(char* name)
{
	struct NamespaceObject* nt = (struct NamespaceObject*)NamespaceObject_New();
	nt->name = name;
	return (struct Object*)nt;
}

struct Object* NamespaceObject_GetAttr(struct Object* self, struct Object* attr)
{
	struct NamespaceObject* ntself = (struct NamespaceObject*)self;
	return DictObject_GetItem(ntself->globals, attr);
}

void NamespaceObject_SetAttr(struct Object* self, struct Object* attr, struct Object* setvalue)
{
	struct NamespaceObject* ntself = (struct NamespaceObject*)self;
	DictObject_SetItem(ntself->globals, attr, setvalue);
}

void NamespaceObject_Print(struct Object* self)
{
	struct NamespaceObject* ntself = (struct NamespaceObject*)self;
	printf("<Namespace %s at %p>", ntself->name,ntself);
}