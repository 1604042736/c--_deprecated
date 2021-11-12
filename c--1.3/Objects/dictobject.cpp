#include "dictobject.h"

Object* DictObject_NewSimple()
{
	DictObject* dictobj = new DictObject;
	dictobj->objattr = &DictObjectAttr;
	return (Object*)dictobj;
}

Object* DictObject_New()
{
	DictObject* dictobj = new DictObject;
	dictobj->objattr = &DictObjectAttr;
	dictobj->objattr->dict = DictObject_NewSimple();

	Object* name = NULL;
	CFunctionObject* cfuncobj = NULL;
	ADDCFUNCTODICT("get", DictObjectDict_GetItem, dictobj->objattr, dictobj);
	ADDCFUNCTODICT("set", DictObjectDict_DictAddItem, dictobj->objattr, dictobj);
	ADDCFUNCTODICT("copy", DictObjectDict_Copy, dictobj->objattr, dictobj);

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
		cout << ',';
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
		if (iter->first->objattr->obj_eq&&iter->first->objattr->obj_eq(iter->first, getobj))
			return iter->second;
		iter++;
	}
	/*Å×³öÒì³£*/
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

Object* DictObjectDict_GetItem(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return DictObject_GetItem(arg1, arg2);
}

Object* DictObjectDict_DictAddItem(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	Object* arg3 = arglist->list[2];
	DictObject_DictAddItem(arg1, arg2, arg3);
	NORMALRETURN;
}

Object* DictObjectDict_Copy(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	return DictObject_Copy(arg1);
}