#include "object.h"

struct Object* Object_New()
{
	struct Object* obj = (struct Object*)malloc(sizeof(struct Object));	//TODO �ڴ����
	obj->objattr = &ObjectObjectAttribute;
	return obj;
}

void Object_Print(struct Object* obj)
{
	printf("%p", obj);
}