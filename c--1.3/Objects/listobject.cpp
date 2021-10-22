#include "listobject.h"

Object* ListObject_New()
{
	ListObject* lobj = new ListObject;
	lobj->objattr = ObjectAttr_New("List");
	lobj->objattr->obj_print = ListObject_Print;
	lobj->objattr->obj_additem = ListObject_AddItem;
	lobj->objattr->obj_finditem = ListObject_FindItem;
	lobj->objattr->obj_getitem = ListObject_GetItem;
	lobj->objattr->obj_insertitem = ListObject_InsertItem;
	lobj->objattr->obj_bool = ListObject_Bool;
	lobj->objattr->obj_copy = ListObject_Copy;
	lobj->objattr->obj_setitem = ListObject_SetItem;
	return (Object*)lobj;
}

void ListObject_Print(Object* obj)
{
	ListObject* lobj = (ListObject*)obj;
	cout << "[";
	for (int i = 0; i < lobj->list.size(); i++)
	{
		print(lobj->list[i]);
		if(i!= lobj->list.size() - 1)
			cout << ",";
	}
	cout << "]";
}

void ListObject_AddItem(Object*obj,Object* addobj)
{
	ListObject* lobj = (ListObject*)obj;
	lobj->list.push_back(addobj);
}

int ListObject_FindItem(Object* obj, Object* findobj)
{
	ListObject* listobj = (ListObject*)obj;
	for (int i = 0; i < listobj->list.size(); i++)
	{
		if (listobj->list[i]->objattr->obj_eq(listobj->list[i], findobj))
		{
			return i;
		}
	}
	return -1;
}

Object* ListObject_GetItem(Object* obj, Object* index)
{
	if (index->objattr->obj_name != "Int")
	{
		//TODO 抛出异常
		return NULL;
	}
	ListObject* listobj = (ListObject*)obj;
	IntObject* indexobj = (IntObject*)index;
	//TODO 对范围的判断
	return listobj->list[indexobj->val];
}

void ListObject_InsertItem(Object* obj, Object* index, Object* insertobj)
{
	if (index->objattr->obj_name != "Int")
	{
		//TODO 抛出异常
		return;
	}
	ListObject* listobj = (ListObject*)obj;
	IntObject* indexobj = (IntObject*)index;
	listobj->list.insert(listobj->list.begin()+indexobj->val, insertobj);
}

bool ListObject_Bool(Object* obj)
{
	ListObject* listobj = (ListObject*)obj;
	return listobj->list.size();
}

Object* ListObject_Copy(Object* obj)
{
	ListObject* listobj = (ListObject*)obj;
	ListObject* copyobj = (ListObject*)ListObject_New();
	for (int i = 0; i < listobj->list.size(); i++)
	{
		copyobj->list.push_back(listobj->list[i]);
	}
	return (Object*)copyobj;
}

void ListObject_SetItem(Object* obj,Object* index,Object* val)
{
	if (index->objattr->obj_name != "Int")
	{
		//TODO 抛出异常
		return;
	}
	ListObject* listobj = (ListObject*)obj;
	IntObject* indexobj = (IntObject*)index;
	listobj->list[indexobj->val] = val;
}