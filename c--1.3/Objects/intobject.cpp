#include "intobject.h"

Object* IntObject_NewFromInt(int val)
{
	IntObject* intobj = new IntObject;
	intobj->val = val;
	intobj->objattr = &IntObjectAttr;
	intobj->objattr->dict = DictObject_NewSimple();

	Object* name = NULL;
	CFunctionObject* cfuncobj = NULL;
	ADDCFUNCTODICT("eq", IntObjectDict_Eq, intobj->objattr, intobj);
	ADDCFUNCTODICT("add", IntObjectDict_Add, intobj->objattr, intobj);
	ADDCFUNCTODICT("sub", IntObjectDict_Sub, intobj->objattr, intobj);
	ADDCFUNCTODICT("mul", IntObjectDict_Mul, intobj->objattr, intobj);
	ADDCFUNCTODICT("div", IntObjectDict_Div, intobj->objattr, intobj);
	ADDCFUNCTODICT("mod", IntObjectDict_Mod, intobj->objattr, intobj);
	ADDCFUNCTODICT("bool", IntObjectDict_Bool, intobj->objattr, intobj);
	ADDCFUNCTODICT("or", IntObjectDict_Or, intobj->objattr, intobj);
	ADDCFUNCTODICT("gt", IntObjectDict_Gt, intobj->objattr, intobj);
	ADDCFUNCTODICT("geq", IntObjectDict_Geq, intobj->objattr, intobj);
	ADDCFUNCTODICT("lt", IntObjectDict_Lt, intobj->objattr, intobj);
	ADDCFUNCTODICT("leq", IntObjectDict_Leq, intobj->objattr, intobj);
	ADDCFUNCTODICT("neq", IntObjectDict_Neq, intobj->objattr, intobj);
	ADDCFUNCTODICT("and", IntObjectDict_And, intobj->objattr, intobj);

	return (Object*)intobj;
}

void IntObject_Print(Object* obj)
{
	IntObject* intobj = (IntObject*)obj;
	cout << intobj->val;
}

bool IntObject_Eq(Object* obj, Object* compareobj)
{
	if (CheckIntObject(obj) || CheckIntObject(compareobj))return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val == compareintobj->val;
}

Object* IntObject_Add(Object* obj, Object* addobj)
{
	IntObject* intobj = (IntObject*)obj;
	if (addobj->objattr->obj_name != "Int")
	{
		//TODO 抛出异常
		return NULL;
	}
	IntObject* intaddobj = (IntObject*)addobj;
	return IntObject_NewFromInt(intobj->val + intaddobj->val);
}

Object* IntObject_Sub(Object* obj, Object* addobj)
{
	IntObject* intobj = (IntObject*)obj;
	if (CheckIntObject(addobj))
	{
		//TODO 抛出异常
		return NULL;
	}
	IntObject* intaddobj = (IntObject*)addobj;
	return IntObject_NewFromInt(intobj->val - intaddobj->val);
}

Object* IntObject_Mul(Object* obj, Object* addobj)
{
	IntObject* intobj = (IntObject*)obj;
	if (CheckIntObject(addobj))
	{
		//TODO 抛出异常
		return NULL;
	}
	IntObject* intaddobj = (IntObject*)addobj;
	return IntObject_NewFromInt(intobj->val * intaddobj->val);
}

Object* IntObject_Div(Object* obj, Object* addobj)
{
	IntObject* intobj = (IntObject*)obj;
	if (CheckIntObject(addobj))
	{
		//TODO 抛出异常
		return NULL;
	}
	IntObject* intaddobj = (IntObject*)addobj;
	return IntObject_NewFromInt(intobj->val / intaddobj->val);
}

Object* IntObject_Mod(Object* obj, Object* addobj)
{
	IntObject* intobj = (IntObject*)obj;
	if (CheckIntObject(addobj))
	{
		//TODO 抛出异常
		return NULL;
	}
	IntObject* intaddobj = (IntObject*)addobj;
	return IntObject_NewFromInt(intobj->val % intaddobj->val);
}

bool IntObject_Bool(Object* obj)
{
	IntObject* intobj = (IntObject*)obj;
	return intobj->val;
}

bool IntObject_Or(Object* obj, Object* compareobj)
{
	if (CheckIntObject(obj) || CheckIntObject(compareobj))return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val || compareintobj->val;
}

bool IntObject_Gt(Object* obj, Object* compareobj)
{
	if (CheckIntObject(obj) || CheckIntObject(compareobj))return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val>compareintobj->val;
}

bool IntObject_Geq(Object* obj, Object* compareobj)
{
	if (CheckIntObject(obj) || CheckIntObject(compareobj))return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val >= compareintobj->val;
}

bool IntObject_Lt(Object* obj, Object* compareobj)
{
	if (CheckIntObject(obj) || CheckIntObject(compareobj))return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val < compareintobj->val;
}

bool IntObject_Leq(Object* obj, Object* compareobj)
{
	if (CheckIntObject(obj) || CheckIntObject(compareobj))return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val <= compareintobj->val;
}

bool IntObject_Neq(Object* obj, Object* compareobj)
{
	if (CheckIntObject(obj) || CheckIntObject(compareobj))return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val != compareintobj->val;
}

bool IntObject_And(Object* obj, Object* compareobj)
{
	if (CheckIntObject(obj) || CheckIntObject(compareobj))return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val && compareintobj->val;
}

Object* IntObjectDict_Eq(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(IntObject_Eq(arg1, arg2));
}

Object* IntObjectDict_Add(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_Add(arg1, arg2);
}

Object* IntObjectDict_Sub(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_Sub(arg1, arg2);
}

Object* IntObjectDict_Mul(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_Mul(arg1, arg2);
}

Object* IntObjectDict_Div(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_Div(arg1, arg2);
}

Object* IntObjectDict_Mod(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_Mod(arg1, arg2);
}

Object* IntObjectDict_Bool(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	return IntObject_NewFromInt(IntObject_Bool(arg1));
}

Object* IntObjectDict_Or(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(IntObject_Or(arg1, arg2));
}

Object* IntObjectDict_Gt(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(IntObject_Gt(arg1, arg2));
}

Object* IntObjectDict_Geq(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(IntObject_Geq(arg1, arg2));
}

Object* IntObjectDict_Lt(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(IntObject_Lt(arg1, arg2));
}

Object* IntObjectDict_Leq(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(IntObject_Leq(arg1, arg2));
}

Object* IntObjectDict_Neq(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(IntObject_Neq(arg1, arg2));
}

Object* IntObjectDict_And(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(IntObject_And(arg1, arg2));
}