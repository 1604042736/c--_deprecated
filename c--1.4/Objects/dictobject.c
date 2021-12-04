#include <string.h>
#include "dictobject.h"
#include "intobject.h"

struct Object* DictObject_New()
{
	struct DictObject* dictobj = Memory_Malloc(memory,"dict");
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
	dictobj->refcount = DEFAULTREFCOUNT;
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
		SUBREFCOUNT(selfdict->item[index].value)
		ADDREFCOUNT(value);
		selfdict->item[index].value = value;
	}
	else
	{
		if (selfdict->size + 1 >= selfdict->allocated)	//超出了已分配的范围
		{
			selfdict->allocated += MAXALLOCATEDSIZE;
			selfdict->item = (struct DictItem*)realloc(selfdict->item ,sizeof(struct DictItem) * selfdict->allocated);
			if (selfdict->item == NULL)
			{
				printf("dict item内存分配失败");
				exit(-1);
			}
		}
		ADDREFCOUNT(key);
		ADDREFCOUNT(value);
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
		struct Object* bl = item->objattr->obj_eq(item, obj);
		if (IntObject_Bool(bl))
		{
			Memory_Free(memory,bl);
			return i;
		}
		Memory_Free(memory, bl);
	}
	return -1;
}

struct Object* DictObject_GetItem(struct Object* self, struct Object* key)
{
	struct DictObject* selfdict = (struct DictObject*)self;
	for (int i = 0; i < selfdict->size; i++)
	{
		struct Object* item = selfdict->item[i].key;
		struct Object* bl = item->objattr->obj_eq(item, key);
		if (IntObject_Bool(bl))
		{
			Memory_Free(memory, bl);
			return selfdict->item[i].value;
		}
		Memory_Free(memory, bl);
	}
	return NULL;
}

int DictObject_Bool(struct Object* self)
{
	struct DictObject* selfdict = (struct DictObject*)self;
	return selfdict->size;
}

void DictObject_DelItem(struct Object* self, struct Object* other)
{
	struct DictObject* selfdict = (struct DictObject*)self;
	int index = DictObject_FindItem(self, other);
	if (index != -1)
	{
		SUBREFCOUNT(selfdict->item[index].key)
		SUBREFCOUNT(selfdict->item[index].value)
		for (int i = index; i < selfdict->size - 1; i++)
		{
			selfdict->item[i] = selfdict->item[i + 1];
		}
		selfdict->size -= 1;
	}
}