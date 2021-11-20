#include "dictobject.h"

struct Object* DictObject_New()
{
	struct DictObject* dictobj = (struct DictObject*)malloc(sizeof(struct DictObject));
	if (dictobj == NULL)
	{
		printf("dict分配内存失败");
		exit(-1);
	}
	dictobj->objattr = &DictObjectAttribute;
	dictobj->allocated = MAXALLOCATEDSIZE;
	dictobj->item = (struct DictItem*)malloc(sizeof(struct DictItem) * dictobj->allocated);
	if (dictobj->item == NULL)
	{
		printf("创建dict时dict item分配内存失败");
		exit(-1);
	}
	dictobj->size = 0;
	return (struct Object*)dictobj;
}

void DictObject_Print(struct Object* obj)
{
	struct DictObject* dictobj = (struct DictObject*)obj;
	printf("{");
	for (int i = 0; i < dictobj->size; i++)
	{
		dictobj->item[i].key->objattr->obj_print(dictobj->item[i].key);
		printf(":");
		dictobj->item[i].value->objattr->obj_print(dictobj->item[i].value);
		if (i < dictobj->size - 1)
		{
			printf(",");
		}
	}
	printf("}");
}

void DictObject_SetItem(struct Object* self, struct Object* key, struct Object* value)
{
	struct DictObject* selfdict = (struct DictObject*)self;
	int index = DictObject_FindItem(self, key);
	if (index != -1)
	{
		selfdict->item[index].value = value;
	}
	else
	{
		if (selfdict->size + 1 >= selfdict->allocated)	//超出了已分配的范围
		{
			struct DictItem* copy = selfdict->item;
			selfdict->allocated += MAXALLOCATEDSIZE;
			selfdict->item = (struct DictItem*)malloc(sizeof(struct DictItem) * selfdict->allocated);
			if (selfdict->item == NULL)
			{
				printf("dict item内存分配失败");
				exit(-1);
			}
			for (int i = 0; i < selfdict->size; i++)
			{
				selfdict->item[i] = copy[i];
			}
		}
		selfdict->item[selfdict->size].key = key;
		selfdict->item[selfdict->size].value = value;
		selfdict->size += 1;
	}
}

int DictObject_FindItem(struct Object* self, struct Object* obj)
{
	struct DictObject* selfdict = (struct DictObject*)self;
	for (int i = 0; i < selfdict->size; i++)
	{
		struct Object* item = selfdict->item[i].key;
		if (item->objattr->obj_eq(item, obj))
		{
			return i;
		}
	}
	return -1;
}

struct Object* DictObject_GetItem(struct Object* self, struct Object* key)
{
	struct DictObject* selfdict = (struct DictObject*)self;
	for (int i = 0; i < selfdict->size; i++)
	{
		struct Object* item = selfdict->item[i].key;
		if (item->objattr->obj_eq(item, key))
		{
			return selfdict->item[i].value;
		}
	}
	return NULL;
}

int DictObject_Bool(struct Object* self)
{
	struct DictObject* selfdict = (struct DictObject*)self;
	return selfdict->size;
}