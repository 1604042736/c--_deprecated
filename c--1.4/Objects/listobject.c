#include "listobject.h"
#include "intobject.h"

struct Object* ListObject_New()
{
	struct ListObject* listobj = (struct ListObject*)malloc(sizeof(struct ListObject));
	if (listobj == NULL)
	{
		printf("list内存分配失败");
		exit(-1);
	}
	listobj->objattr = &ListObjectAttribute;
	listobj->allocated = MAXALLOCATEDSIZE;
	listobj->item = (struct Object**)malloc(sizeof(struct Object) * listobj->allocated);
	if (listobj->item == NULL)
	{
		printf("创建list时list item分配内存失败");
		exit(-1);
	}
	listobj->size = 0;
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
		selflist->item = (struct Object**)malloc(sizeof(struct Object) * selflist->allocated);
		if (selflist->item == NULL)
		{
			printf("list item分配内存失败");
			exit(-1);
		}
		for (int i = 0; i < selflist->size; i++)
		{
			selflist->item[i] = copy[i];
		}
	}
	selflist->size += 1;
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
		if (item!=NULL&&item->objattr->obj_eq!=NULL&& item->objattr->obj_eq(item, obj))
		{
			return i;
		}
	}
	return -1;
}

void ListObject_ListDelItem(struct Object* self, int index)
{
	struct ListObject* selflist = (struct ListObject*)self;
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