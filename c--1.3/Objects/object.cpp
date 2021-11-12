#include "object.h"
#include "dictobject.h"
#include "listobject.h"

/*ObjectAttr* ObjectAttr_New(string name)
{
	ObjectAttr* objattr = new ObjectAttr;
	objattr->obj_name = name;
	objattr->obj_print = Object_Print;
	objattr->obj_additem = Object_AddItem;
	objattr->obj_finditem = Object_FindItem;
	objattr->obj_getitem = Object_GetItem;
	objattr->obj_eq = Object_Eq;
	objattr->obj_dictadditem = Object_DictAddItem;
	objattr->obj_add = Object_Add;
	objattr->obj_sub = Object_Sub;
	objattr->obj_mul = Object_Mul;
	objattr->obj_div = Object_Div;
	objattr->obj_mod = Object_Mod;
	objattr->obj_insertitem = Object_InsertItem;
	objattr->obj_bool = Object_Bool;
	objattr->obj_copy = Object_Copy;
	objattr->obj_or = Object_Or;
	objattr->obj_setitem = Object_SetItem;
	objattr->obj_gt = Object_Gt;
	objattr->obj_geq = Object_Geq;
	objattr->obj_lt = Object_Lt;
	objattr->obj_leq = Object_Leq;
	objattr->obj_neq = Object_Neq;
	objattr->obj_and = Object_And;
	return objattr;
}

Object* Object_New()
{
	Object* obj = new Object;
	obj->objattr = ObjectAttr_New("object");
	return obj;
}

void Object_Print(Object*)
{

}

void Object_AddItem(Object*, Object*)
{

}

int Object_FindItem(Object*, Object*)
{
	return -1;
}

bool Object_Eq(Object*, Object*)
{
	return false;
}

Object* Object_GetItem(Object*, Object*)
{
	return NULL;
}

void Object_DictAddItem(Object*, Object*, Object*)
{

}

Object* Object_Add(Object*, Object*)
{
	return NULL;
}

Object* Object_Sub(Object*, Object*)
{
	return NULL;
}

Object* Object_Mul(Object*, Object*)
{
	return NULL;
}

Object* Object_Div(Object*, Object*)
{
	return NULL;
}

Object* Object_Mod(Object*, Object*)
{
	return NULL;
}

void Object_InsertItem(Object*, Object*, Object*)
{

}

bool Object_Bool(Object*)
{
	return false;
}

Object* Object_Copy(Object*)
{
	return NULL;
}

bool Object_Or(Object*, Object*)
{
	return false;
}

void Object_SetItem(Object*, Object*,Object*)
{

}

bool Object_Gt(Object*, Object*)
{
	return false;
}

bool Object_Geq(Object*, Object*)
{
	return false;
}

bool Object_Lt(Object*, Object*)
{
	return false;
}

bool Object_Leq(Object*, Object*)
{
	return false;
}

bool Object_Neq(Object*, Object*)
{
	return false;
}

bool Object_And(Object*, Object*)
{
	return false;
}

Object* ObjectDict_AddItem(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	Object_AddItem(arg1, arg2);
	NORMALRETURN;
}

Object* ObjectDict_FindItem(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	Object* returnval = IntObject_NewFromInt(Object_FindItem(arg1, arg2));
	return returnval;
}

Object* ObjectDict_GetItem(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return Object_GetItem(arg1, arg2);
}

Object* ObjectDict_Eq(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(Object_Eq(arg1, arg2));
}

Object* ObjectDict_DictAddItem(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	Object* arg3 = arglist->list[2];
	Object_DictAddItem(arg1, arg2,arg3);
	NORMALRETURN;
}

Object* ObjectDict_Add(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return Object_Add(arg1, arg2);
}

Object* ObjectDict_Sub(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return Object_Sub(arg1, arg2);
}

Object* ObjectDict_Mul(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return Object_Mul(arg1, arg2);
}

Object* ObjectDict_Div(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return Object_Div(arg1, arg2);
}

Object* ObjectDict_Mod(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return Object_Mod(arg1, arg2);
}

Object* ObjectDict_InsertItem(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	Object* arg3 = arglist->list[2];
	Object_InsertItem(arg1, arg2,arg3);
	NORMALRETURN;
}

Object* ObjectDict_Bool(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	return IntObject_NewFromInt(Object_Bool(arg1));
}

Object* ObjectDict_Copy(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	return Object_Copy(arg1);
}

Object* ObjectDict_Or(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(Object_Or(arg1, arg2));
}

Object* ObjectDict_SetItem(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	Object* arg3 = arglist->list[2];
	Object_DictAddItem(arg1, arg2, arg3);
	NORMALRETURN;
}

Object* ObjectDict_Gt(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(Object_Gt(arg1, arg2));
}

Object* ObjectDict_Geq(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(Object_Geq(arg1, arg2));
}

Object* ObjectDict_Lt(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(Object_Lt(arg1, arg2));
}

Object* ObjectDict_Leq(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(Object_Leq(arg1, arg2));
}

Object* ObjectDict_Neq(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(Object_Neq(arg1, arg2));
}

Object* ObjectDict_And(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	return IntObject_NewFromInt(Object_And(arg1, arg2));
}*/

void print(Object* obj)
{
	if(obj!=NULL)
		obj->objattr->obj_print(obj);
}