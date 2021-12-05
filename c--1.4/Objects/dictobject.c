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
	for (int i = 0; i < dictobj->allocated; i++)
	{
		dictobj->item[i].key = NULL;
	}
	dictobj->size = 0;
	dictobj->refcount = DEFAULTREFCOUNT;
	//dictobj->hash = ListObject_New();
	return (struct Object*)dictobj;
}

void DictObject_Print(struct Object* obj)
{
	struct DictObject* dictobj = (struct DictObject*)obj;
	printf("{");
	for (int i = 0; i < dictobj->hash->size; i++)
	{
		int index = IntObject_toInt(dictobj->hash->item[i]);
		struct Object* key = dictobj->item[index].key;
		struct Object* value = dictobj->item[index].value;
		key->objattr->obj_print(key);
		printf(":");
		value->objattr->obj_print(value);
		if (i < dictobj->hash->size - 1)
		{
			printf(",");
		}
	}
	printf("}");
}

void DictObject_SetItem(struct Object* self, struct Object* key, struct Object* value)
{
	if (key == NULL || value == NULL)return;
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
			dictresize(selfdict);
		}
		int hash = key->objattr->obj_hash(key) % (selfdict->allocated + 1);
		ADDREFCOUNT(key);
		ADDREFCOUNT(value);
		if (selfdict->item[hash].key == NULL)
		{
			selfdict->item[hash].key = key;
			selfdict->item[hash].value = value;
			selfdict->item[hash].hash = hash;
		}
		else
		{
			for (;; hash += SHIFT)
			{
				if (hash > selfdict->allocated)
				{
					dictresize(selfdict);
				}
				if (selfdict->item[hash].key == NULL)
				{
					selfdict->item[hash].key = key;
					selfdict->item[hash].value = value;
					selfdict->item[hash].hash = hash;
					break;
				}
			}
		}
		selfdict->size++;
		//ListObject_InsertItem(selfdict->hash, selfdict->hash->size, IntObject_NewWithValue(hash));
	}
}

void dictresize(struct DictObject* selfdict)
{
	struct DictItem* copy = selfdict->item;
	int size = selfdict->size;
	selfdict->allocated += MAXALLOCATEDSIZE;
	selfdict->item = (struct DictItem*)malloc(sizeof(struct DictItem) * selfdict->allocated);
	if (selfdict->item == NULL)
	{
		printf("dict item内存分配失败");
		exit(-1);
	}
	//Memory_Free(memory, selfdict->hash);
	//selfdict->hash = ListObject_New();
	for (int i = 0; i < selfdict->allocated;i++)
	{
		selfdict->item[i].key = NULL;
	}
	for (int i = 0; i < size; i++)
	{
		copy[i].key->refcount--;
		copy[i].value->refcount--;
		DictObject_SetItem(selfdict, copy[i].key, copy[i].value);
	}
}

int DictObject_FindItem(struct Object* self, struct Object* obj)
{
	struct DictObject* selfdict = (struct DictObject*)self;
	if (obj->objattr->obj_hash != NULL)
	{
		int hash = obj->objattr->obj_hash(obj)%(selfdict->allocated +1);
		struct DictItem item = selfdict->item[hash];
		if (item.key != NULL)
		{
			if (item.key == obj)
			{
				return hash;
			}
			struct Object* bl = item.key->objattr->obj_eq(item.key, obj);
			if (IntObject_Bool(bl))
			{
				Memory_Free(memory, bl);
				return hash;
			}
			Memory_Free(memory, bl);
			/*开放地址法*/
			for (int i = hash;i<selfdict->allocated; i += SHIFT)
			{
				struct DictItem item = selfdict->item[i];
				if (item.key != NULL)
				{
					if (item.key == obj)
					{
						return i;
					}
					struct Object* bl = item.key->objattr->obj_eq(item.key, obj);
					if (IntObject_Bool(bl))
					{
						Memory_Free(memory, bl);
						return i;
					}
					Memory_Free(memory, bl);
				}
			}
		}
	}
	return -1;
}

struct Object* DictObject_GetItem(struct Object* self, struct Object* key)
{
	struct DictObject* selfdict = (struct DictObject*)self;
	int index = DictObject_FindItem(self, key);
	if (index != -1)
	{
		return selfdict->item[index].value;
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
		selfdict->item[index].key = NULL;
		selfdict->size -= 1;
	}
}