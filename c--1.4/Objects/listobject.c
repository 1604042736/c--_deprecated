#include "listobject.h"
#include "intobject.h"

struct Object* ListObject_New()
{
	struct ListObject* listobj = Memory_Malloc(memory,"list");
	if (listobj == NULL)
	{
		printf("list内存分配失败");
		exit(-1);
	}
	listobj->objattr = &ListObjectAttribute;
	listobj->allocated = MAXALLOCATEDSIZE;
	listobj->item = (struct Object**)malloc(sizeof(struct Object*) * listobj->allocated);
	if (listobj->item == NULL)
	{
		printf("创建list时list item分配内存失败");
		exit(-1);
	}
	listobj->size = 0;
	listobj->objattr->attr = listobjectattr;
	listobj->refcount = DEFAULTREFCOUNT;
	return (struct Object*)listobj;
}

void ListObject_Print(struct Object* obj)
{
	struct ListObject* listobj = (struct ListObject*)obj;
	printf("[");
	for (int i = 0; i < listobj->size; i++)
	{
		if (listobj->item[i]!=NULL&&listobj->item[i]->objattr->obj_print != NULL)
		{
			listobj->item[i]->objattr->obj_print(listobj->item[i]);
		}
		if (i < listobj->size - 1)
		{
			printf(",");
		}
	}
	printf("]");
}

void ListObject_InsertItem(struct Object* self, int index, struct Object* item)
{
	struct ListObject* selflist = (struct ListObject*)self;
	if (index > selflist->size)
	{
		//ERROR
		exit(-1);
	}
	if (selflist->size + 1 >= selflist->allocated)	//超出了已分配的范围
	{
		struct Object** copy = selflist->item;
		selflist->allocated += MAXALLOCATEDSIZE;
		selflist->item = (struct Object**)realloc(selflist->item,sizeof(struct Object*) * selflist->allocated);
		if (selflist->item == NULL)
		{
			printf("list item分配内存失败");
			exit(-1);
		}
	}
	selflist->size += 1;
	ADDREFCOUNT(item);
	for (int i = selflist->size-1; i > index; i--)
	{
		selflist->item[i] = selflist->item[i - 1];
	}
	selflist->item[index] = item;
}

void ListObject_SetItem(struct Object* self, struct Object* index, struct Object* item)
{
	if (!CHECK(index, "int"))
	{
		//error
		exit(-1);
	}
	struct IntObject* indexint = (struct IntObject*)index;
	struct ListObject* selflist = (struct ListObject*)self;
	if (indexint->value >= selflist->size)
	{
		//error
		exit(-1);
	}
	selflist->item[indexint->value] = item;
}

int ListObject_FindItem(struct Object* self, struct Object* obj)
{
	struct ListObject* selflist = (struct ListObject*)self;
	for (int i = 0; i < selflist->size; i++)
	{
		struct Object* item = selflist->item[i];
		if (item!=NULL&&item->objattr->obj_eq!=NULL&& IntObject_Bool(item->objattr->obj_eq(item, obj)))
		{
			return i;
		}
	}
	return -1;
}

void ListObject_ListDelItem(struct Object* self, int index)
{
	struct ListObject* selflist = (struct ListObject*)self;
	SUBREFCOUNT(selflist->item[index])
	for (int i = index; i < selflist->size-1; i++)
	{
		selflist->item[i] = selflist->item[i + 1];
	}
	selflist->size -= 1;
}

int ListObject_Bool(struct Object* self)
{
	struct ListObject* selflist = (struct ListObject*)self;
	return selflist->size;
}

struct Object* ListObject_GetItem(struct Object* self, struct Object* index)
{
	struct ListObject* selflist = (struct ListObject*)self;
	if (!CHECK(index, "int"))
	{
		//TODO
		exit(-1);
	}
	struct IntObject* indexint = (struct IntObject*)index;
	return selflist->item[indexint->value];
}

struct Object* ListObject_GetAttr(struct Object* self, struct Object* attr)
{
	if (!CHECK(attr, "string"))
	{
		//ERROR
		exit(-1);
	}
	return DictObject_GetItem(self->objattr->attr, attr);
}

void ListObject_SetAttr(struct Object* self, struct Object* attr, struct Object* value)
{

}

struct Object* ListObject_InsertItem2(struct Object* args)
{
	struct ListObject* argslist = (struct ListObject*)args;
	if (!CHECK(argslist->item[1], "int"))
	{
		//ERROR
		exit(-1);
	}
	ListObject_InsertItem(argslist->item[0], ((struct IntObject*)argslist->item[1])->value, argslist->item[2]);
	NORMALRETURN
}

struct Object* ListObject_ListDelItem2(struct Object* args)
{
	struct ListObject* argslist = (struct ListObject*)args;
	if (!CHECK(argslist->item[1], "int"))
	{
		//ERROR
		exit(-1);
	}
	ListObject_ListDelItem(argslist->item[0], ((struct IntObject*)argslist->item[1])->value);
	NORMALRETURN
}