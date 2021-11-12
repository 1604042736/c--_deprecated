#include "listobject.h"

Object* ListObject_NewSimple()
{
	ListObject* lobj = new ListObject;
	lobj->objattr = &ListObjectAttr;
	lobj->objattr->dict = DictObject_NewSimple();
	return (Object*)lobj;
}

Object* ListObject_New()
{
	ListObject* lobj = new ListObject;
	lobj->objattr = &ListObjectAttr;
	lobj->objattr->dict = DictObject_NewSimple();

	Object* name = NULL;
	CFunctionObject* cfuncobj = NULL;

	ADDCFUNCTODICT("append", ListObjectDict_AddItem, lobj->objattr,lobj);
	ADDCFUNCTODICT("find", ListObjectDict_FindItem, lobj->objattr, lobj);
	ADDCFUNCTODICT("get", ListObjectDict_GetItem, lobj->objattr, lobj);
	ADDCFUNCTODICT("insert", ListObjectDict_InsertItem, lobj->objattr, lobj);
	ADDCFUNCTODICT("bool", ListObjectDict_Bool, lobj->objattr, lobj);
	ADDCFUNCTODICT("copy", ListObjectDict_Copy, lobj->objattr, lobj);
	ADDCFUNCTODICT("set", ListObjectDict_SetItem, lobj->objattr, lobj);

	return (Object*)lobj;
}

void ListObject_Print(Object* obj)
{
	ListObject* lobj = (ListObject*)obj;
	cout << "[";
	int listsize = lobj->list.size();
	for (int i = 0; i < listsize; i++)
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
	int listsize = listobj->list.size();
	for (int i = 0; i < listsize; i++)
	{
		if (listobj->list[i]->objattr->obj_eq&&listobj->list[i]->objattr->obj_eq(listobj->list[i], findobj))
		{
			return i;
		}
	}
	return -1;
}

Object* ListObject_GetItem(Object* obj, Object* index)
{
	if (CheckIntObject(index))
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
	if (CheckIntObject(index))
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
	int listsize = listobj->list.size();
	for (int i = 0; i < listsize; i++)
	{
		copyobj->list.push_back(listobj->list[i]);
	}
	return (Object*)copyobj;
}

void ListObject_SetItem(Object* obj,Object* index,Object* val)
{
	if (CheckIntObject(index))
	{
		//TODO 抛出异常
		return;
	}
	ListObject* listobj = (ListObject*)obj;
	IntObject* indexobj = (IntObject*)index;
	listobj->list[indexobj->val] = val;
}

Object* ListObjectDict_AddItem(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	ListObject_AddItem(arg1, arg2);
	NORMALRETURN;
}

Object* ListObjectDict_FindItem(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(ListObject_FindItem(arg1, arg2));
}

Object* ListObjectDict_GetItem(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return ListObject_GetItem(arg1, arg2);
}

Object* ListObjectDict_InsertItem(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	Object* arg3 = arglist->list[2];
	ListObject_InsertItem(arg1, arg2, arg3);
	NORMALRETURN;
}

Object* ListObjectDict_Bool(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	return IntObject_NewFromInt(ListObject_Bool(arg1));
}

Object* ListObjectDict_Copy(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	return ListObject_Copy(arg1);
}

Object* ListObjectDict_SetItem(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	Object* arg3 = arglist->list[2];
	ListObject_SetItem(arg1, arg2, arg3);
	NORMALRETURN;
}