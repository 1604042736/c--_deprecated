#include "namespaceobject.h"

struct Object* NamespaceObject_New()
{
	struct NamespaceObject* nt = (struct NamespaceObject*)malloc(sizeof(struct NamespaceObject));
	nt->objattr = &NamespaceObjectAttribute;
	nt->globals = DictObject_New();
	return (struct Object*)nt;
}