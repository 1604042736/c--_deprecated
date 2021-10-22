#include "stringobject.h"

Object* StringObject_NewFromString(string s)
{
	StringObject* stringobj = new StringObject;
	stringobj->str = s;
	stringobj->objattr = ObjectAttr_New("String");
	stringobj->objattr->obj_print = StringObject_Print;
	stringobj->objattr->obj_eq = StringObject_Eq;
	stringobj->objattr->obj_add = StringObject_Add;
	stringobj->objattr->obj_bool = StringObject_Bool;
	return (Object*)stringobj;
}

void StringObject_Print(Object* obj)
{
	StringObject* strobj = (StringObject*)obj;
	cout << '"'<<strobj->str<<'"';
}

bool StringObject_Eq(Object* obj, Object* compareobj)
{
	if (obj->objattr->obj_name!="String"|| compareobj->objattr->obj_name != "String")return false;	//类型不一样直接判断为不相等
	StringObject* strobj = (StringObject*)obj;
	StringObject* comparestrobj = (StringObject*)compareobj;
	return strobj->str == comparestrobj->str;
}

Object* StringObject_Add(Object* obj, Object* addobj)
{
	StringObject* strobj = (StringObject*)obj;
	if (addobj->objattr->obj_name != "String")
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