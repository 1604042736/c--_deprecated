#include "intobject.h"

Object* IntObject_NewFromInt(int val)
{
	IntObject* intobj = new IntObject;
	intobj->val = val;
	intobj->objattr = ObjectAttr_New("Int");
	intobj->objattr->obj_print = IntObject_Print;
	intobj->objattr->obj_eq = IntObject_Eq;
	intobj->objattr->obj_add = IntObject_Add;
	intobj->objattr->obj_sub = IntObject_Sub;
	intobj->objattr->obj_mul = IntObject_Mul;
	intobj->objattr->obj_div = IntObject_Div;
	intobj->objattr->obj_mod = IntObject_Mod;
	intobj->objattr->obj_bool = IntObject_Bool;
	intobj->objattr->obj_or = IntObject_Or;
	intobj->objattr->obj_gt = IntObject_Gt;
	intobj->objattr->obj_geq = IntObject_Geq;
	intobj->objattr->obj_lt = IntObject_Lt;
	intobj->objattr->obj_leq = IntObject_Leq;
	intobj->objattr->obj_neq = IntObject_Neq;
	intobj->objattr->obj_and= IntObject_And;
	return (Object*)intobj;
}

void IntObject_Print(Object* obj)
{
	IntObject* intobj = (IntObject*)obj;
	cout << intobj->val;
}

bool IntObject_Eq(Object* obj, Object* compareobj)
{
	if (obj->objattr->obj_name != "Int" || compareobj->objattr->obj_name != "Int")return false;	//类型不一样直接判断为不相等
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
	if (addobj->objattr->obj_name != "Int")
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
	if (addobj->objattr->obj_name != "Int")
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
	if (addobj->objattr->obj_name != "Int")
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
	if (addobj->objattr->obj_name != "Int")
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
	if (obj->objattr->obj_name != "Int" || compareobj->objattr->obj_name != "Int")return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val || compareintobj->val;
}

bool IntObject_Gt(Object* obj, Object* compareobj)
{
	if (obj->objattr->obj_name != "Int" || compareobj->objattr->obj_name != "Int")return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val>compareintobj->val;
}

bool IntObject_Geq(Object* obj, Object* compareobj)
{
	if (obj->objattr->obj_name != "Int" || compareobj->objattr->obj_name != "Int")return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val >= compareintobj->val;
}

bool IntObject_Lt(Object* obj, Object* compareobj)
{
	if (obj->objattr->obj_name != "Int" || compareobj->objattr->obj_name != "Int")return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val < compareintobj->val;
}

bool IntObject_Leq(Object* obj, Object* compareobj)
{
	if (obj->objattr->obj_name != "Int" || compareobj->objattr->obj_name != "Int")return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val <= compareintobj->val;
}

bool IntObject_Neq(Object* obj, Object* compareobj)
{
	if (obj->objattr->obj_name != "Int" || compareobj->objattr->obj_name != "Int")return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val != compareintobj->val;
}

bool IntObject_And(Object* obj, Object* compareobj)
{
	if (obj->objattr->obj_name != "Int" || compareobj->objattr->obj_name != "Int")return false;	//类型不一样直接判断为不相等
	IntObject* intobj = (IntObject*)obj;
	IntObject* compareintobj = (IntObject*)compareobj;
	return intobj->val && compareintobj->val;
}