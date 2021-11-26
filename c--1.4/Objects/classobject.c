#include "classobject.h"

struct Object* ClassObject_New()
{
	struct ClassObject* classobj = (struct ClassObject*)malloc(sizeof(struct ClassObject));
	classobj->objattr = &ClassObjectAttribute;
	classobj->bases = ListObject_New();
	classobj->name = StringObject_New();
	classobj->dict = DictObject_New();
	return (struct Object*)classobj;
}

void ClassObject_Print(struct Object* self)
{
	struct ClassObject* ct = (struct ClassObject*)self;
	printf("<Class %s at %p>", ct->name->string, ct);
}

struct Object* ClassObject_Call(struct Object* self, struct Object* args)
{
	struct ClassObject* ct = (struct ClassObject*)self;

	struct InstanceObject* it = InstanceObject_New();
	it->objattr->attr = DictObject_New();
	it->classname = ct->name;
	for (int i = 0; i < ct->bases->size; i++)
	{
		struct Object* obj = ct->bases->item[i]->objattr->obj_new();
		struct DictObject* attr = (struct DictObject*)obj->objattr->attr;
		for (int i = 0; i < attr->size; i++)
		{
			DictObject_SetItem(it->objattr->attr, attr->item[i].key, attr->item[i].value);
		}
	}
	for (int i = 0; i < ct->dict->size; i++)
	{
		DictObject_SetItem(it->objattr->attr, ct->dict->item[i].key, ct->dict->item[i].value);
	}
	return it;
}

struct Object* InstanceObject_New()
{
	struct InstanceObject* classobj = (struct InstanceObject*)malloc(sizeof(struct InstanceObject));
	classobj->objattr = &InstanceObjectAttribute;
	return (struct Object*)classobj;
}

void InstanceObject_Print(struct Object* self)
{
	struct InstanceObject* ct = (struct InstanceObject*)self;
	printf("<Instance %s at %p>", ct->classname->string, ct);
}

struct Object* InstanceObject_GetAttr(struct Object* self, struct Object* attr)
{
	if (!CHECK(attr, "string"))
	{
		//ERROR
		exit(-1);
	}
	return DictObject_GetItem(self->objattr->attr, attr);
}

void InstanceObject_SetAttr(struct Object* self, struct Object* attr, struct Object* setvalue)
{
	DictObject_SetItem(self->objattr->attr, attr, setvalue);
}