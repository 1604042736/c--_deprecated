#include "dictobject.h"

Object* DictObject_New()
{
	DictObject* dictobj = new DictObject;
	dictobj->objattr = ObjectAttr_New("Dict");
	dictobj->objattr->obj_print = DictObject_Print;
	dictobj->objattr->obj_getitem = DictObject_GetItem;
	dictobj->objattr->obj_dictadditem = DictObject_DictAddItem;
	dictobj->objattr->obj_copy = DictObject_Copy;
	dictobj->objattr->obj_setitem = DictObject_DictAddItem;
	return (Object*)dictobj;
}

void DictObject_Print(Object* obj)
{
	DictObject* dictobj = (DictObject*)obj;
	map<Object*, Object*>::iterator iter;
	iter = dictobj->dict.begin();
	cout << "{";
	while (iter != dictobj->dict.end())
	{
		print(iter->first);
		cout << ": ";
		print(iter->second);
		iter++;
	}
	cout << "}";
}

Object* DictObject_GetItem(Object* obj, Object* getobj)
{
	DictObject* dictobj = (DictObject*)obj;
	map<Object*, Object*>::iterator iter;
	iter = dictobj->dict.begin();
	while (iter != dictobj->dict.end())
	{
		if (iter->first->objattr->obj_eq(iter->first, getobj))
			return iter->second;
		iter++;
	}
	return NULL;
}

void DictObject_DictAddItem(Object* obj, Object* keyobj,Object* valueobj)
{
	DictObject* dictobj = (DictObject*)obj;
	dictobj->dict[keyobj] = valueobj;
}

Object* DictObject_Copy(Object* obj)
{
	DictObject* dictobj = (DictObject*)obj;
	Object* copyobj = DictObject_New();
	map<Object*, Object*>::iterator iter;
	iter = dictobj->dict.begin();
	while (iter != dictobj->dict.end())
	{
		copyobj->objattr->obj_dictadditem(copyobj, iter->first, iter->second);
		iter++;
	}
	return copyobj;
}