#include "stringobject.h"

Object* StringObject_NewFromStringSimple(string s)
{
	StringObject* stringobj = new StringObject;
	stringobj->str = s;
	stringobj->objattr = &StringObjectAttr;
	return (Object*)stringobj;
}

Object* StringObject_NewFromString(string s)
{
	StringObject* stringobj = new StringObject;
	stringobj->str = s;
	stringobj->objattr = &StringObjectAttr;
	stringobj->objattr->dict = DictObject_NewSimple();

	Object* name = NULL;
	CFunctionObject* cfuncobj = NULL;
	ADDCFUNCTODICT("eq", StringObjectDict_Eq, stringobj->objattr, stringobj);
	ADDCFUNCTODICT("add", StringObjectDict_Add, stringobj->objattr, stringobj);
	ADDCFUNCTODICT("bool", StringObjectDict_Bool, stringobj->objattr, stringobj);

	return (Object*)stringobj;
}

void StringObject_Print(Object* obj)
{
	StringObject* strobj = (StringObject*)obj;
	cout << strobj->str;
}

bool StringObject_Eq(Object* obj, Object* compareobj)
{
	if (CheckStringObject(obj)|| CheckStringObject(compareobj))return false;	//类型不一样直接判断为不相等
	StringObject* strobj = (StringObject*)obj;
	StringObject* comparestrobj = (StringObject*)compareobj;
	return strobj->str == comparestrobj->str;
}

Object* StringObject_Add(Object* obj, Object* addobj)
{
	StringObject* strobj = (StringObject*)obj;
	if (CheckStringObject(addobj))
	{
		//TODO 抛出异常
		return NULL;
	}
	StringObject* straddobj = (StringObject*)addobj;
	return StringObject_NewFromString(strobj->str + straddobj->str);
}

bool StringObject_Bool(Object* obj)
{
	StringObject* strobj = (StringObject*)obj;
	return strobj->str.length();
}

Object* StringObjectDict_Eq(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(StringObject_Eq(arg1, arg2));
}

Object* StringObjectDict_Add(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return StringObject_Add(arg1, arg2);
}

Object* StringObjectDict_Bool(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	return IntObject_NewFromInt(StringObject_Bool(arg1));
}